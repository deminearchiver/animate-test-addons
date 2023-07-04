#pragma once

#define ANIMATE_SHORT(value) (unsigned short*)(value)
#define ANIMATE_WIDE(value) (const wchar_t*)(value)
#define ANIMATE_OPTIONAL(method_ptr, fallback, ...) (method_ptr ? ((*method_ptr)(__VA_ARGS__)) : fallback)

#define ANIMATE_METHOD(name) animate::jsapi::Boolean name(animate::jsapi::Context* context, animate::jsapi::Object* object, unsigned int argc, animate::jsapi::Value* argv, animate::jsapi::Value* return_value)
#define ANIMATE_REGISTER(method, argc) animate::jsapi::Register(L#method, method, argc)

#define ANIMATE_QUICK(name, args, body) \
auto name = ([](animate::jsapi::Context* context, animate::jsapi::Object* object, unsigned int argc, animate::jsapi::Value* argv, animate::jsapi::Value* return_value) -> animate::jsapi::Boolean body); \
animate::jsapi::Register(L#name, name, args);

#define ANIMATE_WIDE2(value) L##value
#define ANIMATE_WIDE1(value) ANIMATE_WIDE2(value)
#define ANIMATE_WIDE_FILE ANIMATE_WIDE1(__FILE__)
#define ANIMATE_EVAL(context, object, script, return_value) animate::jsapi::Eval(context, object, script, return_value, ANIMATE_WIDE_FILE, __LINE__)
#define ANIMATE_EVAL_PRIVATE(context, object, script, return_value) animate::jsapi::Eval(context, object, script, return_value)

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
          unsigned short* (*ValueToString)(Context* context, Value value, unsigned int* length_ptr);
          unsigned char* (*ValueToBytes)(Context* context, Value value, unsigned int* length_ptr);
          Boolean (*ValueToInteger)(Context* context, Value value, long long* long_ptr);
          Boolean (*ValueToDouble)(Context* context, Value value, double* double_ptr);
          Boolean (*ValueToBoolean)(Context* context, Value value, Boolean* boolean_ptr);
          Boolean (*ValueToObject)(Context* context, Value value, Object** object_ptr);
          Boolean (*StringToValue)(Context* context, unsigned short* string, unsigned int size, Value* value);
          Boolean (*BytesToValue)(Context* context, unsigned char* bytes, unsigned int size, Value* value);
          Boolean (*DoubleToValue)(Context* context, double number, Value* value);
          unsigned short* (*ObjectType)(Object* object);
          Object* (*NewArrayObject)(Context* context, unsigned int length, Value* value);
          long long (*GetArrayLength)(Context* context, Object* object);
          Boolean (*GetElement)(Context* context, Object* object, unsigned int index, Value* value);
          Boolean (*SetElement)(Context* context, Object* object, unsigned int index, Value* value);
          Boolean (*ExecuteScript)(Context* context, Object* object, unsigned short* script, unsigned int size, unsigned short* file, unsigned int line, Value* return_value);
          Boolean (*ReportError)(Context* context, unsigned short* error, unsigned int size);
        } Environment;
        extern Environment env;

        inline const wchar_t* ValueToString(Context* context, Value value, unsigned int* length_ptr) {
            return ANIMATE_WIDE(ANIMATE_OPTIONAL(env.ValueToString, nullptr, context, value, length_ptr));
        }
        inline Boolean StringToValue(Context* context, const wchar_t* string, Value* value) {
            return ANIMATE_OPTIONAL(env.StringToValue, false, context, ANIMATE_SHORT(string), 0, value);
        }

        inline Boolean ValueToInteger(Context* context, Value value, long long* long_ptr) {
            return ANIMATE_OPTIONAL(env.ValueToInteger, false, context, value, long_ptr);
        }
        inline Value IntegerToValue(long long number) {
            return ((Value) number << 1) | 0x1;
        }

        inline Boolean ValueToDouble(Context* context, Value value, double* double_ptr) {
            return ANIMATE_OPTIONAL(env.ValueToDouble, false, context, value, double_ptr);
        }
        inline Boolean DoubleToValue(Context* context, double number, Value* value) {
            return ANIMATE_OPTIONAL(env.DoubleToValue, false, context, number, value);
        }

        inline Boolean ValueToBoolean(Context* context, Value value, Boolean* boolean_ptr) {
            return ANIMATE_OPTIONAL(env.ValueToBoolean, false, context, value, boolean_ptr);
        }
        inline Value BooleanToValue(Boolean boolean) {
            return ((Value) boolean << 3) | 0x6;
        }

        inline Boolean ValueToObject(Context* context, Value value, Object** object_ptr) {
            return ANIMATE_OPTIONAL(env.ValueToObject, false, context, value, object_ptr);
        }
        inline Value ObjectToValue(Object* object) {
            return (Value) object;
        }

        inline const wchar_t* ObjectType(Object* object) {
            return ANIMATE_WIDE(ANIMATE_OPTIONAL(env.ObjectType, nullptr, object));
        }



        inline Boolean Register(const wchar_t* name, APIMethod method, unsigned int argc) {
            return ANIMATE_OPTIONAL(env.DefineFunction, false, env.library, ANIMATE_SHORT(name), method, argc);
        }

        // TODO: do something with __FILE__ and __LINE__
        inline Boolean Eval(Context* context, Object* object, const wchar_t* script, Value* return_value, const wchar_t* file, unsigned int line) {
            return ANIMATE_OPTIONAL(env.ExecuteScript, false, context, object, ANIMATE_SHORT(script), 0, ANIMATE_SHORT(file), line, return_value);
        }
        inline Boolean Eval(Context* context, Object* object, const wchar_t* script, Value* return_value) {
            return Eval(context, object, script, return_value, L"", 0);
        }

        inline Boolean Throw(Context* context, const wchar_t* error) {
            return ANIMATE_OPTIONAL(env.ReportError, false, context, ANIMATE_SHORT(error), 0);
        }
    }
}

#undef ANIMATE_SHORT
#undef ANIMATE_SHORT
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