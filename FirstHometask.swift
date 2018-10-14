import Foundation

protocol BInterface {
    func bar()
    func foo()
}

protocol Subscriber {
    func subscriptionUpdated()
}

final class A: NSObject, Subscriber {
    func subscriptionUpdated() {
        print(self)
    }
}

@objcMembers
final class B: NSObject, BInterface {
    func bar() {
        print(#function)
    }
    
    func foo() {
        print(#function)
    }
    
    @objc dynamic var observableValue = 0
}

final class Proxy {}
final class Mediator {}
final class Observer {}

final class C<ClassType> {
    private final class Subscription: NSObject {
        
        init(subscriber: Subscriber, object: B) {
            self.subscriber = subscriber
            self.object = object
            self.keyPaths = Mirror(reflecting: self.object).children.compactMap { $0.label }
            super.init()
            keyPaths.forEach { self.object.addObserver(self, forKeyPath: $0, options: [.old, .new], context: nil) }
        }
        
        deinit {
            keyPaths.forEach { self.object.removeObserver(self, forKeyPath: $0) }
        }
        
        override func observeValue(forKeyPath keyPath: String?,
                                   of object: Any?,
                                   change: [NSKeyValueChangeKey : Any]?,
                                   context: UnsafeMutableRawPointer?) {
            guard object is B else { return }
            subscriber.subscriptionUpdated()
        }
        
        private let subscriber: Subscriber
        private let object: B
        private let keyPaths: [String]
    }
    
    private var slaves = [B]()
    private var slave = B()
    private var subscriptions = [Subscription]()
}

extension C : BInterface where ClassType == Proxy {
    func bar() {
        slave.bar()
    }
    
    func foo() {
        slave.foo()
    }
}

extension C where ClassType == Mediator {
    func add(object: B) {
        slaves.append(object)
    }
    
    func clear() {
        slaves.removeAll()
    }
    
    func call(caller: AnyObject, selector: Selector) -> AnyObject? {
        guard let freeSlave = getNextFreeSlave(), caller is A else { return nil }
        guard freeSlave.responds(to: selector) else { return nil }
        return freeSlave.perform(selector)?.takeRetainedValue()
    }
    
    private func getNextFreeSlave() -> B? {
        return slaves.randomElement()
    }
}

extension C where ClassType == Observer {
    func addSubscription(to object: B, subscriber: A) {
        subscriptions.append(Subscription(subscriber: subscriber, object: object))
    }
}

// Testing proxy
print("Testing proxy:")
let proxy = C<Proxy>()
proxy.bar()
proxy.foo()

// Testing mediator
print("Testing mediator:")
let mediator = C<Mediator>()
let master = A()
let slave = B()

mediator.add(object: slave)

mediator.call(caller: master, selector: #selector(B.bar))
mediator.call(caller: master, selector: #selector(B.foo))

// Testing observer
print("Testing observer:")
let observer = C<Observer>()
let anotherMaster = A()
observer.addSubscription(to: slave, subscriber: master)
observer.addSubscription(to: slave, subscriber: anotherMaster)
slave.observableValue = 1
