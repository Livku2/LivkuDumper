#include <jni.h>
#include <string>
#include "thread"
#include "fstream"
#include "dlfcn.h"
#include "vector"
#include "android/log.h"
#include "flags.h"
#include "Dobby/include/dobby.h"
#include "KittyInclude.hpp"
#include "sstream"
#include "unistd.h"
#include "Il2Cpp.hpp"

using namespace std;

#define _Tag "LivkuDumper"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, _Tag, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, _Tag, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, _Tag, __VA_ARGS__)

ProcMap map;

string GenerateCPPTypeName(const char* typeName){
    if (strcmp(typeName, "Void") == 0 || strcmp(typeName, "System.Void") == 0) {
        return "void";
    }
    if (strcmp(typeName, "Single") == 0 || strcmp(typeName, "System.Single") == 0) {
        return "float";
    }
    if (strcmp(typeName, "Int32") == 0 || strcmp(typeName, "System.Int32") == 0) {
        return "int";
    }
    if (strcmp(typeName, "Int16") == 0 || strcmp(typeName, "System.Int16") == 0) {
        return "short";
    }
    if (strcmp(typeName, "Boolean") == 0 || strcmp(typeName, "System.Boolean") == 0) {
        return "bool";
    }
    if (strcmp(typeName, "Double") == 0 || strcmp(typeName, "System.Double") == 0) {
        return "double";
    }
    if (strcmp(typeName, "UInt64") == 0 || strcmp(typeName, "System.UInt64") == 0) {
        return "ulong";
    }
    if (strcmp(typeName, "String") == 0 || strcmp(typeName, "System.String") == 0) {
        return "string";
    }
    if (strcmp(typeName, "System.IntPtr") == 0 || strcmp(typeName, "IntPtr") == 0) {
        return "IntPtr";
    }
    if (strcmp(typeName, "System.Char") == 0 || strcmp(typeName, "Char") == 0) {
        return "char";
    }
    if (strcmp(typeName, "System.Byte") == 0 || strcmp(typeName, "Byte") == 0) {
        return "byte";
    }
    if (strcmp(typeName, "System.Int64") == 0 || strcmp(typeName, "Int64") == 0) {
        return "long";
    }
    if (strcmp(typeName, "System.SByte") == 0 || strcmp(typeName, "SByte") == 0) {
        return "sbyte";
    }

    return typeName;
}

namespace InternalSettings{
    bool gameHasThreadCheck;
}
namespace Globals{
    void* libIl2cppHandle = nullptr;
    size_t assemblyCount;
    const void ** assemblies;
    bool hasDumped = false;
}

std::string getMethodModifier(uint32_t flags) {
    std::stringstream outPut;
    auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;
    switch (access) {
        case METHOD_ATTRIBUTE_PRIVATE:
            outPut << "private ";
            break;
        case METHOD_ATTRIBUTE_PUBLIC:
            outPut << "public ";
            break;
        case METHOD_ATTRIBUTE_FAMILY:
            outPut << "protected ";
            break;
        case METHOD_ATTRIBUTE_ASSEM:
        case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
            outPut << "internal ";
            break;
        case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
            outPut << "protected internal ";
            break;
    }
    if (flags & METHOD_ATTRIBUTE_STATIC) {
        outPut << "static ";
    }
    if (flags & METHOD_ATTRIBUTE_ABSTRACT) {
        outPut << "abstract ";
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_FINAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT) {
            outPut << "sealed override ";
        }
    } else if (flags & METHOD_ATTRIBUTE_VIRTUAL) {
        if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT) {
            outPut << "virtual ";
        } else {
            outPut << "override ";
        }
    }
    if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL) {
        outPut << "extern ";
    }
    return outPut.str();
}

string DumpFields(const void *klass){
    stringstream output;

    void *iter = nullptr;
    while (auto field = Il2Cpp::il2cpp_class_get_fields(const_cast<void*>(klass), &iter)) {

        output << "\t\t";

        auto fieldName = Il2Cpp::il2cpp_field_get_name(field);

        auto type = Il2Cpp::il2cpp_field_get_type(field);

        auto isEnum = Il2Cpp::il2cpp_class_is_enum(klass);

        auto field_class = Il2Cpp::il2cpp_class_from_type(type);

        auto attrs = Il2Cpp::il2cpp_field_get_flags(field);
        auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
        switch (access) {
            case FIELD_ATTRIBUTE_PRIVATE:
                output << "private ";
                break;
            case FIELD_ATTRIBUTE_PUBLIC:
                output << "public ";
                break;
            case FIELD_ATTRIBUTE_FAMILY:
                output << "protected ";
                break;
            case FIELD_ATTRIBUTE_ASSEMBLY:
            case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
                output << "internal ";
                break;
            case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
                output << "protected internal ";
                break;
        }
        if (attrs & FIELD_ATTRIBUTE_LITERAL) {
            output << "const ";
        } else {
            if (attrs & FIELD_ATTRIBUTE_STATIC) {
                output << "static ";
            }
            if (attrs & FIELD_ATTRIBUTE_INIT_ONLY) {
                output << "readonly ";
            }
        }

        auto fixedName = GenerateCPPTypeName(Il2Cpp::il2cpp_class_get_name(field_class));

        if (attrs & FIELD_ATTRIBUTE_LITERAL && isEnum) {
            uint64_t val = 0;
            Il2Cpp::il2cpp_field_static_get_value(field, &val);
            output << fieldName << " = " << std::dec << val << "\n";
        }
        else{
            output << fixedName << " " << fieldName << "; // 0x" << std::hex << Il2Cpp::il2cpp_field_get_offset(field) << "\n";
        }
    }

    return output.str();
}

string DumpMethods(const void* klass){
    stringstream output;

    void *iter = nullptr;

    while (auto method = Il2Cpp::il2cpp_class_get_methods(const_cast<void*>(klass), &iter)) {

        auto methodName = Il2Cpp::il2cpp_method_get_name(method);

        bool isCtor = strcmp(methodName, ".ctor") == 0;

        auto returnType = Il2Cpp::il2cpp_method_get_return_type(method);
        auto classFromType = Il2Cpp::il2cpp_class_from_type(const_cast<void *>(returnType));
        auto returnTypeName = Il2Cpp::il2cpp_class_get_name(classFromType);

        if (isCtor) {
            methodName = Il2Cpp::il2cpp_class_get_name(const_cast<void*>(klass));
            returnTypeName = "";
        }

        auto fixedName = GenerateCPPTypeName(returnTypeName);

        auto methodPointer = *(void**) method;

        auto va = reinterpret_cast<uint64_t>(methodPointer);
        uint64_t offset = va - map.startAddress;

        uint32_t iflags = 0;
        auto flags = Il2Cpp::il2cpp_method_get_flags(method, &iflags);
        stringstream argStream;

        int paramCount = Il2Cpp::il2cpp_method_get_param_count(method);
        for (int i = 0; i < paramCount; i++){
            auto parameterType = Il2Cpp::il2cpp_method_get_param(const_cast<void *>(method), i);

            char* parameterTypeName = Il2Cpp::il2cpp_type_get_name(parameterType);

            auto paramName = Il2Cpp::il2cpp_method_get_param_name(method, i);

            argStream << parameterTypeName << " " << paramName;
            if(i < paramCount - 1) {
                argStream << ", ";
            }
        }

        auto args = argStream.str();

        output << "\t\t// VA: 0x" << std::hex << va
               << " | RVA: 0x" << std::hex << offset << "\n\t\t"
               << getMethodModifier(flags) << fixedName << " "
               << methodName << "(" << args << ")" << " {\t} \n \n";
    }
    return output.str();
}

string DumpImage(const void* image){
    stringstream output;
    auto classCount = Il2Cpp::il2cpp_image_get_class_count(image);
    for(int i = 0; i < classCount; ++i){
        auto klass = Il2Cpp::il2cpp_image_get_class(image, i);
        auto namespaze = Il2Cpp::il2cpp_class_get_namespace(const_cast<void*>(klass));
        auto name = Il2Cpp::il2cpp_class_get_name(const_cast<void*>(klass));
        auto methods = DumpMethods(klass);
        auto fields = DumpFields(klass);

        string type;
        if(Il2Cpp::il2cpp_class_is_enum(klass)){
            type = "enum ";
        }
        else{
            type = "class ";
        }

        auto parent = Il2Cpp::il2cpp_class_get_parent(const_cast<void*>(klass));
        bool hasParent = (parent != nullptr);


        output << "//" << Il2Cpp::il2cpp_image_get_name(image);

        output <<
        "\n" << "namespace " << namespaze << "\n{\n" <<
        "\t" << type << name;

        if(hasParent){
            output << " : " << Il2Cpp::il2cpp_class_get_name(parent);
        }

        output << "\n\t{\n" <<
        methods << fields <<
        "\t}\n" <<
        "}\n";

    }
    return output.str();
}

const void* GetImageAtCount(size_t count){
    auto assembly = Globals::assemblies[count];
    if(!assembly){
        LOGE("Assembly Not Found At Count %i", static_cast<int>(count));
        return nullptr;
    }

    auto image = Il2Cpp::il2cpp_assembly_get_image(assembly);

    return image;
}

const char *GetPackageName() {
    char *application_id[256];
    FILE *fp = fopen("proc/self/cmdline", "r");
    if (fp) {
        fread(application_id, sizeof(application_id), 1, fp);
        fclose(fp);
    }
    return (const char *) application_id;
}

void SetupAssemblies(){
    auto domain = Il2Cpp::il2cpp_domain_get();
    auto assemblies = Il2Cpp::il2cpp_domain_get_assemblies(domain, &Globals::assemblyCount);
    Globals::assemblies = assemblies;
}

//Thread for bypass
void DumpThread(){
    thread([](){

        LOGI("DUMPING...");
        if(InternalSettings::gameHasThreadCheck){
            LOGI("GAME HAS THREAD CHECK");
        }

        stringstream imagesStream;
        stringstream dumpStream;

        imagesStream << "Game Has Thread Check: " << InternalSettings::gameHasThreadCheck << "\n";

        LOGI("ASSEMBLY COUNT: %i", Globals::assemblyCount);

        for(int i = 0; i < Globals::assemblyCount; ++i){
            auto currentImage = GetImageAtCount(i);
            if(!currentImage){
                LOGE("Image Is Nullptr At Count %i", i);
                continue;
            }

            imagesStream << "Image " << i << ": " << Il2Cpp::il2cpp_image_get_name(currentImage) << "\n";

            dumpStream << DumpImage(currentImage);
        }

        stringstream fullStream;
        fullStream << imagesStream.str() << dumpStream.str();

        auto directory = std::string("/storage/emulated/0/Android/data/").append(
                GetPackageName()).append("/dump.cs");

        std::ofstream outputStream(directory);
        outputStream << fullStream.str();
        outputStream.close();

        LOGI("Dump Finished");

    }).detach();
}

void* (*ClassFromName)(const void*, const char*, const char*) = nullptr;
void* ClassFromNameHook(const void* image, const char* namespaze, const char* name){
    DumpThread();
    DobbyDestroy(reinterpret_cast<void*>(ClassFromNameHook));
    return ClassFromName(image, namespaze, name);
}

void InitThreadCheckBypass(){
    if(!Globals::libIl2cppHandle){
        LOGE("Handle is null, this should not happen");
        return;
    }

    auto il2cpp_class_from_name = dlsym(Globals::libIl2cppHandle, "il2cpp_class_from_name");
    if(!il2cpp_class_from_name){
        LOGE("il2cpp_class_from_name is null");
        return;
    }
    DobbyHook(il2cpp_class_from_name, reinterpret_cast<dobby_dummy_func_t>(ClassFromNameHook), reinterpret_cast<dobby_dummy_func_t*>(&ClassFromName));
}

void Init(){

    auto domain = Il2Cpp::il2cpp_domain_get();

    InternalSettings::gameHasThreadCheck = domain == nullptr;

    InitThreadCheckBypass();
}

__attribute__ ((constructor))
void lib_main(){
    std::thread([](){
        LOGI("Loading libil2cpp");
        do{
            map = KittyMemory::getElfBaseMap("libil2cpp.so");
            sleep(1);
        }while (!map.isValid() || !map.isValidELF());

        Globals::libIl2cppHandle = dlopen("libil2cpp.so", RTLD_LAZY);
        bool il2cppInitialised = false;
        do
        {
            il2cppInitialised = Il2Cpp::Init(Globals::libIl2cppHandle);
        }while (!il2cppInitialised);
        Init();

    }).detach();
}
