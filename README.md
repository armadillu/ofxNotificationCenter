# ofxNotificationCenter

[![Build Status](https://travis-ci.org/local-projects/ofxNotificationCenter.svg?branch=master)](https://travis-ci.org/local-projects/ofxNotificationCenter)


The idea behind this is to have a "Mailbox" anyone can refer to to send and receive messages.
To receive notifications, one must first register itself to do so.
There are different notification "channels" - one can "post to" and/or "subscribe for" to a certain
channel/type of notifications. Notifications are distinguished by an ID(std::string) the developer supplies.

This is pretty much a copy of [NSNotification](https://developer.apple.com/documentation/foundation/nsnotificationcenter) found in the Cocoa interfaces.

For a Notification to happen, we need at least 2 entities:

* the Notification Poster: someone who posts a notification on a certain channel
* A Notification Observer: someone who is listening for notifications in a certain channel

Notifications include an ID and an ofJson object for basic data:

```c++
struct Notification{
	string ID;
	ofJson data;
};
```

Developers who want to send extra custom-type data inside their notifications should make their own Notifications by subclassing `ofxNotificationCenter::Notification`:

```c++
struct MyNotification : public ofxNotificationCenter::Notification{
	//your stuff here
};
```

## Example

We want to keep track of a device being plugged in / out across different components of our app.
ofApp owns the pointer to the device, and it's the only component that gets the events from the device.
We need that event to be forwarded to several components of our app that are buried under several levels of
encapsulation.

The workflow to solve this problem with [ofxNotificationCenter](https://github.com/armadillu/ofxNotificationCenter) would be:

1. **(Optional)** Define a custom data structure (subclassing `ofxNotificationCenter::Notification`) to be able attach your custom data into your notifications:

	```c++
	struct MyNotificationData : public ofxNotificationCenter::Notification {
		MyObjectType * myObject;
	};
	```
	**NOTE:** This step is only required if you need to send custom objects in your notifications, otherwise the ofJson `data` field available in `ofxNotificationCenter::Notification`might suffice.
	
2. Make sure all the required Observers "tune in" for notifications of your desired Notification ID:

	```c++
	//in the setup() of each of the objects that wants to be notified for this Notification ID
	ofxNotificationCenter::one().addObserver(this, &MyClass::onMyNotification, "MyNotificationType");
	```


3. Post notifications from wherever you need to do so:
 
	```c++
	ofxNotificationCenter::Notification mnd; //Create a notification object to send along
	mnd.data["myCustomField"] = "MyCustomData"; //add as many generic data types as you need
	mnd.data["myFloatVector"] = (vector<float>){0.2, 0.5, 0.6}; //some standard std structures also supported
	
	//Post the notification	
	ofxNotificationCenter::one().postNotification("MyNotificationType", mnd);
	```
	
	If you need to use custom data types, your notification object should be of your custom data type (Requires step 1).
	
	```c++
	MyNotificationData myCustomNotificationData;
	myCustomNotificationData.myObject = mySpecialObjectPtr; 
		
		//Post the notification	
	ofxNotificationCenter::one().postNotification("MyNotificationType", mnd);
	```

4. Implement a notification callback for the observers to receive notifications:

	```c++	 
	void MyClass::onMyNotification(ofxNotificationCenter::Notification & n){

		string notificationID = n.ID; 
		string myField = n.data["myCustomField"]; //get what you need from the data field in the Notification
		vector<float> myVector = n.data["myFloatVector"];
		
		//if you require custom data types and your notification poster sent a custom type
		//like `MyNotificationData`, obtain your custom notification data by dynamic casting
		try{
			MyNotificationData & n = dynamic_cast<MyNotificationData&>(n);
			MyObjectType * mySpecialObjectPtr = n.myObject;
		}catch(const std::bad_cast& e){
			ofLogError() << "type conversion error!";
		}
	 }
	```

 
5. When an Observer does not want to listen to more notifications (or before it is delete) it must remove
 itself from the observer list. This is step is very important, otherwise the application will *crash*.
 
 ```c++
 ofxNotificationCenter::one().removeObserver(this, &MyClass::onMyNotification, "MyNotificationType");
 ```
