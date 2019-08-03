#include <node.h>
#define DLL_IMPORT __declspec(dllimport)
using dll.dll;
// Crée une méthode

void json_generation(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  dll.Lceb(args);
}

//On associe le nom 'sum' à la fonction Sum et on l'exporte.
void Initialize(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports,"json_gen",json_generation);
}
NODE_MODULE(addon,Initialize);
