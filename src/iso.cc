#include "iso.h"

using namespace v8;
using namespace node;

Persistent<FunctionTemplate> iso::s_ct;

void iso::Initialize(Handle<Object> target)
{
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(NODE_SYMBOL("iso"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "run", Run);
    NODE_SET_PROTOTYPE_METHOD(s_ct, "close", Close);

    target->Set(NODE_SYMBOL("iso"), t->GetFunction());
}

Handle<Value> iso::New (const Arguments &args)
{
    HandleScope scope;
    if (!args.IsConstructCall())
    {
        return ThrowException(Exception::TypeError(NODE_SYMBOL("Use the new operator to create instances of this object.")));
    }
    ARG_CHECK_FUNCTION(0, 'task');

    iso *isola = new iso();
    isola->_task = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
    isola->_isolate = Isolate::New();
    isola->Wrap(args.This());
    return scope.Close(args.This());
}

/* FUNCTIONS */
Handle<Value> iso::Run(const Arguments &args)
{
    // Check args
    ARG_CHECK_OPTIONAL_FUNCTION(0, 'callback');
    ARG_CHECK_OPTIONAL_OBJECT(1, 'scope');

    //Unwrap class
    iso *isola = ObjectWrap::Unwrap<iso>(args.This());
    assert(isola);
    assert(isola->_isolate);

    //Set up isolate
    V8::Initialize();
    HandleScope scope;
    Persistent<Context> ctx = Context::New();
    Context::Scope context_scope(ctx);

    //Parse callback
    unsigned fnct = 0;
    Local<Value> fnargs[1];
    if(args.Length() > 0) {
        fnct = 1;
        fnargs[0] = Local<Function>::Cast(args[0]);
        assert(fnargs[0]->IsFunction());
    }

    //Parse scope
    Handle<Object> uscope;
    if(args.Length() > 1) {
     uscope = Handle<Object>::Cast(args[0]->ToObject());
    } else {
     uscope = ctx->Global();
    }

    //Run function
    TryCatch try_catch;
    Handle<Value> result = isola->_task->Call(uscope, fnct, fnargs);
    if (try_catch.HasCaught()) {
        FatalException(try_catch);
    }
    return scope.Close(result);
}
Handle<Value> iso::Close(const Arguments &args)
{
    HandleScope scope;
    iso *isola = ObjectWrap::Unwrap<iso>(args.This());
    assert(isola);
    assert(isola->_isolate);
    isola->_isolate->Dispose();
    return scope.Close(args.This());
}

extern "C" {
    static void init (Handle<Object> target)
    {
        HandleScope scope;
        iso::Initialize(target);
    }

    NODE_MODULE(iso, init);
}
