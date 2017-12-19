#pragma once

#include "ofMain.h"
#include "ofxInterface.h"
#include "ofxNotificationCenter.h"

/// See "ofxNotificationCenter.h" for an explanation on how ofxNotificationCenter works

////////////////////////////////////////////////////////////////////////////////////////////////////

// Build my own notification data structure to attach to notifications

struct MyNotificationData : public ofxNotificationCenter::Notification {

	string myInfo;
	float myValue;

	MyNotificationData(){
		ofLogNotice("MyNotificationData") << "MyNotificationData()";
	}

	~MyNotificationData(){
		ofLogNotice("MyNotificationData") << "~MyNotificationData()";
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////


class MyNode : public ofxInterface::Node{
public:

	void setup(float x, float y, float w, float h){
		setPosition(x, y);
		setSize(w, h);
		/// Register myself (this) for Notifications of type "deviceConnectedNotification" and "deviceDisconnectedNotification"
		ofxNotificationCenter & notifCenter = ofxNotificationCenter::one();
		notifCenter.addObserver(this, &MyNode::onDeviceConnectedNotif, "deviceConnectedNotification");
		notifCenter.addObserver(this, &MyNode::onDeviceDisconnectedNotif, "deviceDisconnectedNotification");
	}

	void drawDebug(){
		ofxInterface::Node::drawDebug();
		ofDrawBitmapStringHighlight(deviceAvailable ? "Device Connected" : "Device Disconnected", 20, 20);
	}

	void onDeviceConnectedNotif(ofxNotificationCenter::Notification& n){
		ofLogNotice("MyNode") << "onDeviceConnectedNotif()";
		deviceAvailable = true;

		//obtain your notification data by dynamic casting
		try{
			MyNotificationData & n2 = dynamic_cast<MyNotificationData&>(n);
			ofLogNotice() << n2.myInfo << " " << n2.myValue;
		}catch(const std::bad_cast& e){
			ofLogError() << "type conversion error!";
		}
	}

	void onDeviceDisconnectedNotif(ofxNotificationCenter::Notification& n){
		ofLogNotice("MyNode") << "onDeviceDisconnectedNotif()";
		deviceAvailable = false;

		//obtain your notification data by dynamic casting
		try{
			MyNotificationData & n2 = dynamic_cast<MyNotificationData&>(n);
			ofLogNotice() << n2.myInfo << " " << n2.myValue;
		}catch(const std::bad_cast& e){
			ofLogError() << "type conversion error!";
		}
	}

	bool deviceAvailable = false;
};


////////////////////////////////////////////////////////////////////////////////////////////////////


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		ofxInterface::Node* scene;

		void populateScene();

		void connectDevice();
		void disconnectDevice();
};


////////////////////////////////////////////////////////////////////////////////////////////////////
