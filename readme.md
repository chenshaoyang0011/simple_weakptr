This project is a simple weak ptr implemention learn from chromium/base/weak_ptr.h

Weak pointers are pointers to an object that do not affect its lifetime,
and which may be invalidated (i.e. reset to nullptr) by the object, or its
owner, at any time, most commonly when the object is about to be deleted.

Weak pointers are useful when an object needs to be accessed safely by one
or more objects other than its owner, and those callers can cope with the
object vanishing and e.g. tasks posted to it being silently dropped.
Reference-counting such an object would complicate the ownership graph and
make it harder to reason about the object's lifetime.

EXAMPLE:
```C++
 class Controller {
  public:
   void SpawnWorker() { Worker::StartNew(weak_factory_.GetWeakPtr()); }
   void WorkComplete(const Result& result) { ... }
  private:
   // Member variables should appear before the WeakPtrFactory, to ensure
   // that any WeakPtrs to Controller are invalidated before its members
   // variable's destructors are executed, rendering them invalid.
   WeakPtrFactory<Controller> weak_factory_{this};
 };

 class Worker {
  public:
   static void StartNew(WeakPtr<Controller> controller) {
     // Move WeakPtr when possible to avoid atomic refcounting churn on its
     // internal state.
     Worker* worker = new Worker(std::move(controller));
     // Kick off asynchronous processing...
   }
  private:
   Worker(WeakPtr<Controller> controller)
       : controller_(std::move(controller)) {}
   void DidCompleteAsynchronousProcessing(const Result& result) {
     if (controller_)
       controller_->WorkComplete(result);
   }
   WeakPtr<Controller> controller_;
 };
```

With this implementation a caller may use SpawnWorker() to dispatch multiple
Workers and subsequently delete the Controller, without waiting for all
Workers to have completed.