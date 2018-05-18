//
//  ofxNotificationCenter.h
//  ofxInterface_exampleAdvanced
//
//  Created by Oriol Ferrer Mesià on 09/10/2017.
//
//

#pragma once
#include "ofMain.h"

class ofxNotificationCenter{

public:

	static ofxNotificationCenter& one(){
		static ofxNotificationCenter instance;
		return instance;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	//this is the data that's attached to every notification
	//it supports basic types through ofJson
	//you should subclass this to attach custom types to your notifications
	struct Notification{

		string ID;
		ofJson data; //support random data embedding (basic types only)

		virtual ~Notification(){}; //make Notification a polymorphic object
		Notification(){};

		template<typename T>
		Notification(const string &key, const T & value){ //constructor shortcut with key-value combo
			data[key] = value;
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////

	template <class ListenerClass>
	void addObserver(ListenerClass * who,
					 void (ListenerClass::*callback)(Notification &),
					 const string & notificationID
					 ){

		auto it = eventsByID.find(notificationID);
		if(it == eventsByID.end()){
			eventsByID[notificationID] = ofEvent<Notification>();
		}

		auto & event = eventsByID[notificationID];
		ofAddListener( event, who, callback );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	template <class ListenerClass>
	void removeObserver(ListenerClass * who,
						void (ListenerClass::*callback)(Notification &),
						const string & notificationID){

		auto it = eventsByID.find(notificationID);
		if(it != eventsByID.end()){
			auto & event = it->second;
			ofRemoveListener( event, who, callback );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	void postNotification(const string & notificationID){
		Notification notif;
		postNotification(notificationID, notif);
	}

	void postNotification(const string & notificationID, Notification & notif ){

		auto it = eventsByID.find(notificationID);
		if(it != eventsByID.end()){
			notif.ID = notificationID;
			//ofLogNotice("ofxNotificationCenter") << "posting notification with ID \"" << notificationID << "\"";
			ofNotifyEvent(it->second, notif, this);
		}else{
			ofLogError("ofxNotificationCenter") << "can't post notification with ID \"" << notificationID << "\" because nobody is listening!";
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////

protected:

	ofxNotificationCenter(){};

	map<string, ofEvent<Notification> > eventsByID;

};
