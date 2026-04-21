#include "GlobalSettings.hpp"
#include "Il2CppSymbols.hpp"

inline void* GetExportFunction(void* handle, const char* function) {
    return sym(handle, function);
}

class Il2Cpp
{
public:
    static inline void* (*il2cpp_domain_get)();
    static inline const void** (*il2cpp_domain_get_assemblies)(const void*, size_t*);
    static inline const void* (*il2cpp_assembly_get_image)(const void*);
    static inline size_t (*il2cpp_image_get_class_count)(const void*);
    static inline const char* (*il2cpp_image_get_name)(const void*);
    static inline const void* (*il2cpp_image_get_class)(const void*, size_t);
    static inline const char* (*il2cpp_class_get_name)(void*);
    static inline const char* (*il2cpp_class_get_namespace)(void*);
    static inline const void* (*il2cpp_class_get_methods)(void*, void**);
    static inline const void* (*il2cpp_method_get_return_type)(const void*);
    static inline const char* (*il2cpp_method_get_name)(const void*);
    static inline void* (*il2cpp_class_from_type)(const void*);
    static inline int (*il2cpp_method_get_param_count)(const void*);
    static inline uint32_t (*il2cpp_method_get_flags)(const void*, uint32_t*);
    static inline const void* (*il2cpp_method_get_param)(const void*, uint32_t);
    static inline char* (*il2cpp_type_get_name)(const void*);
    static inline const char* (*il2cpp_method_get_param_name)(const void*, uint32_t);
    static inline void* (*il2cpp_class_get_fields)(void*, void**);
    static inline int (*il2cpp_field_get_flags)(void*);
    static inline const char* (*il2cpp_field_get_name)(void*);
    static inline void* (*il2cpp_field_get_parent)(void*);
    static inline size_t (*il2cpp_field_get_offset)(void*);
    static inline const void* (*il2cpp_field_get_type)(void*);
    static inline void (*il2cpp_field_get_value)(void*, void*, void*);
    static inline void* (*il2cpp_field_get_value_object)(void*, void*);
    static inline bool (*il2cpp_field_has_attribute)(void*, void*);
    static inline void (*il2cpp_field_static_get_value)(void*, void*);
    static inline void (*il2cpp_field_static_set_value)(void*, void*);
    static inline bool (*il2cpp_class_is_enum)(const void*);
    static inline void (*il2cpp_field_set_value_object)(void*, void*, void*);
    static inline void* (*il2cpp_class_get_parent)(void*);
    static inline bool (*il2cpp_method_is_generic)(const void*);

    static bool Init(void* handle)
    {
        if (!handle) return false;

        il2cpp_domain_get = reinterpret_cast<decltype(il2cpp_domain_get)>(
            GetExportFunction(handle, "il2cpp_domain_get"));

        il2cpp_domain_get_assemblies = reinterpret_cast<decltype(il2cpp_domain_get_assemblies)>(
            GetExportFunction(handle, "il2cpp_domain_get_assemblies"));

        il2cpp_assembly_get_image = reinterpret_cast<decltype(il2cpp_assembly_get_image)>(
            GetExportFunction(handle, "il2cpp_assembly_get_image"));

        il2cpp_image_get_class_count = reinterpret_cast<decltype(il2cpp_image_get_class_count)>(
            GetExportFunction(handle, "il2cpp_image_get_class_count"));

        il2cpp_image_get_name = reinterpret_cast<decltype(il2cpp_image_get_name)>(
            GetExportFunction(handle, "il2cpp_image_get_name"));

        il2cpp_image_get_class = reinterpret_cast<decltype(il2cpp_image_get_class)>(
            GetExportFunction(handle, "il2cpp_image_get_class"));

        il2cpp_class_get_name = reinterpret_cast<decltype(il2cpp_class_get_name)>(
            GetExportFunction(handle, "il2cpp_class_get_name"));

        il2cpp_class_get_namespace = reinterpret_cast<decltype(il2cpp_class_get_namespace)>(
            GetExportFunction(handle, "il2cpp_class_get_namespace"));

        il2cpp_class_get_methods = reinterpret_cast<decltype(il2cpp_class_get_methods)>(
            GetExportFunction(handle, "il2cpp_class_get_methods"));

        il2cpp_method_get_return_type = reinterpret_cast<decltype(il2cpp_method_get_return_type)>(
            GetExportFunction(handle, "il2cpp_method_get_return_type"));

        il2cpp_method_get_name = reinterpret_cast<decltype(il2cpp_method_get_name)>(
            GetExportFunction(handle, "il2cpp_method_get_name"));

        il2cpp_class_from_type = reinterpret_cast<decltype(il2cpp_class_from_type)>(
            GetExportFunction(handle, "il2cpp_class_from_type"));

        il2cpp_method_get_param_count = reinterpret_cast<decltype(il2cpp_method_get_param_count)>(
            GetExportFunction(handle, "il2cpp_method_get_param_count"));

        il2cpp_method_get_flags = reinterpret_cast<decltype(il2cpp_method_get_flags)>(
            GetExportFunction(handle, "il2cpp_method_get_flags"));

        il2cpp_method_get_param = reinterpret_cast<decltype(il2cpp_method_get_param)>(
            GetExportFunction(handle, "il2cpp_method_get_param"));

        il2cpp_type_get_name = reinterpret_cast<decltype(il2cpp_type_get_name)>(
            GetExportFunction(handle, "il2cpp_type_get_name"));

        il2cpp_method_get_param_name = reinterpret_cast<decltype(il2cpp_method_get_param_name)>(
            GetExportFunction(handle, "il2cpp_method_get_param_name"));

        il2cpp_class_get_fields = reinterpret_cast<decltype(il2cpp_class_get_fields)>(
            GetExportFunction(handle, "il2cpp_class_get_fields"));

        il2cpp_field_get_flags = reinterpret_cast<decltype(il2cpp_field_get_flags)>(
            GetExportFunction(handle, "il2cpp_field_get_flags"));

        il2cpp_field_get_name = reinterpret_cast<decltype(il2cpp_field_get_name)>(
            GetExportFunction(handle, "il2cpp_field_get_name"));

        il2cpp_field_get_parent = reinterpret_cast<decltype(il2cpp_field_get_parent)>(
            GetExportFunction(handle, "il2cpp_field_get_parent"));

        il2cpp_field_get_offset = reinterpret_cast<decltype(il2cpp_field_get_offset)>(
            GetExportFunction(handle, "il2cpp_field_get_offset"));

        il2cpp_field_get_type = reinterpret_cast<decltype(il2cpp_field_get_type)>(
            GetExportFunction(handle, "il2cpp_field_get_type"));

        il2cpp_field_get_value = reinterpret_cast<decltype(il2cpp_field_get_value)>(
            GetExportFunction(handle, "il2cpp_field_get_value"));

        il2cpp_field_get_value_object = reinterpret_cast<decltype(il2cpp_field_get_value_object)>(
            GetExportFunction(handle, "il2cpp_field_get_value_object"));

        il2cpp_field_has_attribute = reinterpret_cast<decltype(il2cpp_field_has_attribute)>(
            GetExportFunction(handle, "il2cpp_field_has_attribute"));

        il2cpp_field_static_get_value = reinterpret_cast<decltype(il2cpp_field_static_get_value)>(
            GetExportFunction(handle, "il2cpp_field_static_get_value"));

        il2cpp_field_static_set_value = reinterpret_cast<decltype(il2cpp_field_static_set_value)>(
            GetExportFunction(handle, "il2cpp_field_static_set_value"));

        il2cpp_class_is_enum = reinterpret_cast<decltype(il2cpp_class_is_enum)>(
            GetExportFunction(handle, "il2cpp_class_is_enum"));

        il2cpp_field_set_value_object = reinterpret_cast<decltype(il2cpp_field_set_value_object)>(
            GetExportFunction(handle, "il2cpp_field_set_value_object"));

        il2cpp_class_get_parent = reinterpret_cast<decltype(il2cpp_class_get_parent)>(
            GetExportFunction(handle, "il2cpp_class_get_parent"));

        il2cpp_method_is_generic = reinterpret_cast<decltype(il2cpp_method_is_generic)>(
            GetExportFunction(handle, "il2cpp_method_is_generic"));

        return true;
    }
};
