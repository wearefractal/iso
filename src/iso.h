#ifndef ISO_H_
#define ISO_H_

#include <v8.h>
#include <node.h>
#include "node_defs.h"

#include <assert.h>

using namespace v8;
using namespace node;

class iso : public ObjectWrap
{
public:
    static void Initialize(Handle<Object> target);

    /* FUNCTIONS */
    static Handle<Value> Run(const Arguments &args);

private:
    iso() : ObjectWrap() {}
    ~iso() {}
    static Persistent<FunctionTemplate> s_ct;
    static Handle<Value> New(const Arguments &args);
    Isolate* _isolate;
    Persistent<Function> _task;
};

#endif  // ISO_H_
