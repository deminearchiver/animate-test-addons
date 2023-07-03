#pragma once

#define WIDE_TO_SHORT(value) (unsigned short*)(value)
#define SHORT_TO_WIDE(value) (const wchar_t*)(value)
#define ANIMATE_OPTIONAL(method_ptr, fallback, ...) (method_ptr ? ((*method_ptr)(__VA_ARGS__)) : fallback)

#define ANIMATE_METHOD(name) animate::jsapi::Boolean name(animate::jsapi::Context* context, animate::jsapi::Object* object, unsigned int argc, animate::jsapi::Value* argv, animate::jsapi::Value* return_value)
#define ANIMATE_REGISTER(method, argc) animate::jsapi::Register(L#method, method, argc)

#define ANIMATE_QUICK(name, args, body) \
auto name = ([](animate::jsapi::Context* context, animate::jsapi::Object* object, unsigned int argc, animate::jsapi::Value* argv, animate::jsapi::Value* return_value) -> animate::jsapi::Boolean body); \
animate::jsapi::Register(L#name, name, args);

namespace animate {
    namespace jsapi {
        typedef struct Object Object;
        typedef struct Context Context;

        typedef long long Boolean;
        typedef long long Value;

        typedef Boolean (*APIMethod)(Context* context, Object* object, unsigned int argc, Value* argv, Value* returnValue);

        typedef struct {
          Object* library;
          Boolean (*DefineFunction)(Object* library, unsigned short* name, APIMethod method, unsigned int argc);
          unsigned short* (*ValueToString)(Context* context, Value value, unsigned int* length);
          unsigned char* (*ValueToBytes)(Context* context, Value value, unsigned int* length);
          Boolean (*ValueToInteger)(Context* context, Value value, long* long_ptr);
          Boolean (*ValueToDouble)(Context* context, Value value, double* double_ptr);
          Boolean (*ValueToBoolean)(Context* context, Value value, Boolean* boolean_ptr);
          Boolean (*ValueToObject)(Context* context, Value value, Object** object_ptr);
          Boolean (*StringToValue)(Context* context, unsigned short* string, unsigned int size, Value* value);
          Boolean (*BytesToValue)(Context* context, unsigned char* bytes, unsigned int size, Value* value);
          Boolean (*DoubleToValue)(Context* context, double number, Value* value);
          unsigned short* (*ObjectType)(Object* object);
          Object* (*NewArrayObject)(Context* context, unsigned int length, Value* value);
          long (*GetArrayLength)(Context* context, Object* object);
          Boolean (*GetElement)(Context* context, Object* object, unsigned int index, Value* value);
          Boolean (*SetElement)(Context* context, Object* object, unsigned int index, Value* value);
          Boolean (*ExecuteScript)(Context* context, Object* object, unsigned short* script, unsigned int size, unsigned short* file, unsigned int line, Value* return_value);
          Boolean (*ReportError)(Context* context, unsigned short* error, unsigned int size);
        } Environment;
        extern Environment env;

        inline Boolean Register(const wchar_t* name, APIMethod method, unsigned int argc) {
            return ANIMATE_OPTIONAL(env.DefineFunction, false, env.library, WIDE_TO_SHORT(name), method, argc);
        }
    }
}

#undef WIDE_TO_SHORT
#undef SHORT_TO_WIDE
#undef ANIMATE_OPTIONAL




#define ANIMATE_INIT(init_function) \
animate::jsapi::Environment animate::jsapi::env; \
void init_function(); \
extern "C" __declspec(dllexport) void MM_InitWrapper(animate::jsapi::Environment* external, unsigned int env_size) { \
    char** external_ptr = (char**)external; \
    char** env_ptr =(char**)&animate::jsapi::env;\
    char** external_end = (char**)((char*)external_ptr + env_size); \
    char** env_end =(char**)((char*)env_ptr + sizeof(animate::jsapi::Environment)); \
    while (env_ptr < env_end && external_ptr < external_end) *env_ptr++ = *external_ptr++; \
    while (env_ptr < env_end) *env_ptr++ = nullptr; \
    return init_function(); \
}


#define ANIMATE_TERMINATE(terminate_function) \
extern int terminate_function(); \
extern "C" __declspec(dllexport) int MM_Terminate() { \
    return terminate_function(); \
}