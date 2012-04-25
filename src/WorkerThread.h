#ifndef WORKERTHREAD_H_
#define WORKERTHREAD_H_

#include <v8.h>

using namespace v8;

class WorkerThread {
public:
    WorkerThread(Isolate* isolate, Handle<Context> context, Handle<Object> scope, Handle<Function> task, unsigned argc, Handle<Value> *args) 
        : isolate_(isolate), context_(context), scope_(scope), task_(task), argc_(argc), args_(args) {}
    void Run() 
    {
        {
            Locker locker(isolate_);
            Isolate::Scope isolate_scope(isolate_);
            HandleScope scope;
            Context::Scope context_scope(context_);
            Local<Value> result = task_->Call(scope_, argc_, args_);
        }
        {
            Locker locker(isolate_);
            Isolate::Scope isolate_scope(isolate_);
            Context::Scope context_scope(context_);
            HandleScope scope;
            Local<Value> result = task_->Call(scope_, argc_, args_);
        }
        isolate_->Dispose();
    }

private:
    Isolate* isolate_;
    Persistent<Context> context_;
    Persistent<Object> scope_;
    Persistent<Function> task_;
    unsigned argc_;
    Handle<Value> *args_;

};

#endif  // WORKERTHREAD_H_
