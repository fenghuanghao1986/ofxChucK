#include "ofApp.h"

#include <stdio.h>
#include <math.h>
#include <cmath>

//------------------------------------------------------------------------------
// name: setup()
// desc: set up the app
//------------------------------------------------------------------------------
void ofApp::setup()
{
    ofSetLogLevel( OF_LOG_VERBOSE );
    ofEnableDepthTest();
    ofSetVerticalSync( false );
    
    showOverlay = false;
    
    // CHUCK STUFF -------------------------------------------------------------
    
    // get singleton
    chuck = TheChucK::instance();
    // arguments
    const char * argv[] = { "the", "-v0" };
    
    // initialize (SHOULD HAPPEN BEFORE AUDIO STREAM STARTS)
    chuck->initialize( MY_SRATE, MY_BUFFERSIZE, MY_CHANNELS_IN, MY_CHANNELS_OUT, 2, argv );
    
    // set pointer
    vr = VR::instance();
    
    // set up light
    m_light = new ofLight();
    m_light->setDiffuseColor( ofColor(100, 255, 100) );
    m_light->setGlobalPosition( 1000, 1000, 1000 );
    m_light->enable();
    
    // success code
    bool r = false;
    // compile and run file
    //r = chuck->compileFile( "ck/dot-circle.ck" );
    //r = chuck->compileFile( "ck/solar.ck" );
    // r = chuck->compileFile( "ck/lines.ck" );
    // r = chuck->compileFile( "ck/text.ck" );
    //r = chuck->compileFile( "ck/flares.ck" ); // need audio input
    //r = chuck->compileFile( "ck/points.ck" );
    // r = chuck->compileFile( "ck/head.ck" );
    r = chuck->compileFile( "ck/mesh.ck" );
    // r = chuck->compileFile( "ck/turenas.ck" );
    
    // check
    if( !r )
    {
        // set error
        m_message = "ERROR compiling ChucK code\n(check console for details...)";
    }
    
    // setup the sound stream...
    soundStream.setup( this,
                      MY_CHANNELS_OUT, // output
                      MY_CHANNELS_IN,  // input
                      MY_SRATE,        // sample rate
                      MY_BUFFERSIZE,   // buffer size
                      MY_NUMBUFFERS ); // num buffer
    
    
    // OCULUS STUFF ------------------------------------------------------------
    
    oculusRift.baseCamera = &cam;
    oculusRift.setup();
    oculusRift.fullscreenOnRift();
    
    //TODO: delete
    //    if (ofIsGLProgrammableRenderer())
    //        bshader.load("Shaders_GL3/simple.vert", "Shaders_GL3/simple.frag");
    
    // ofBox uses texture coordinates from 0-1, so you can load whatever
    // sized images you want and still use them to texture your box
    // but we have to explicitly normalize our tex coords here
    ofEnableNormalizedTexCoords();
    
    //enable mouse;
    cam.setAutoDistance(false);
    cam.begin();
    cam.end();
    
    // set camera y to user eye height
    cam.setGlobalPosition(0, oculusRift.getUserEyeHeight(), 3);
    
    
    //------------
    arial.loadFont(ofToDataPath("font/Arial.ttf"), 14, true, true);
    arial.setLineHeight(18.0f);
    arial.setLetterSpacing(1.037);
}




//------------------------------------------------------------------------------
// name: update()
// desc: update the system
//------------------------------------------------------------------------------
void ofApp::update()
{
    // set background
    ofBackground( 1, 1, 1 );
    
    // head quaternion
    ofQuaternion headtrack;
    // some vectors
    ofVec3f euler, headpos;
    // head tracking as quaternion
    headtrack = oculusRift.getOrientationQuat();
    // headpos = oculusRift.getTranslation();
    // get euler angles
    euler = headtrack.getEuler();

    // synchronize
    VR::instance()->lock();
    
    // get head
    VREntity * head = vr->head();
    // set orientation
    head->ori.x = euler[0];
    head->ori.y = euler[1];
    head->ori.z = euler[2];
    
    // update it
    vr->root()->updateAll(1/60.0f);
    // update it
    vr->head()->updateAll(1/60.0f);
    
    // trigger displaySync to chuck
    chuck->displaySync();
    
    // sychronize
    VR::instance()->release();
}

//------------------------------------------------------------------------------
// name: draw()
// desc: draw the scene for OculusDK2. uses drawScene() for each eye.
//------------------------------------------------------------------------------
void ofApp::draw()
{
    // rift
    if(oculusRift.isSetup())
    {
        // overlay
        if(showOverlay)
        {
            oculusRift.beginOverlay(-230, 320,240);
            ofRectangle overlayRect = oculusRift.getOverlayRectangle();
            
            ofPushStyle();
            ofEnableAlphaBlending();
            ofFill();
            ofSetColor(255, 40, 10, 200);
            
            ofRect(overlayRect);
            
            ofSetColor(255,255);
            ofFill();
            
            ofSetColor(0, 255, 0);
            ofNoFill();
            ofCircle(overlayRect.getCenter(), 20);
            
            ofPopStyle();
            oculusRift.endOverlay();
        }
        
        // render scene for left eye
        oculusRift.beginLeftEye();
        drawScene();
        oculusRift.endLeftEye();

        // render scene for right eye
        oculusRift.beginRightEye();
        drawScene();
        oculusRift.endRightEye();

        // draw
        oculusRift.draw();
    }
    else // normal
    {
        cam.begin();
        drawScene();
        cam.end();
    }
}


//------------------------------------------------------------------------------
// name: drawScene()
// desc: draw the scene, not taking into account anything OculusDK2-related.
//------------------------------------------------------------------------------
void ofApp::drawScene()
{
    
    // check
    if( m_message != "" )
    {
        // draw it
        ofDrawBitmapString( m_message, 30, 30, 0 );
    }
    
    ofPushMatrix();
    ofRotate(90, 0, 0, -1);
    ofSetColor(30);
    ofDrawGridPlane(12.0f, 8.0f, false );
    ofPopMatrix();
    
    
    // Draw HMD ori
//    ofPushMatrix();
//    oculusRift.beginOverlay();
//    ofRotateX(180);
//    ofColor(255, 140, 100);
//    char tmp_str[255];
//    sprintf(tmp_str,
//            "Ori x: %f y: %f z: %f w: %f",
//            oculusRift.headPose[0].Orientation.x,
//            oculusRift.headPose[0].Orientation.y,
//            oculusRift.headPose[0].Orientation.z,
//            oculusRift.headPose[0].Orientation.w);
//    arial.drawString(tmp_str, 0, 0);
//    oculusRift.endOverlay();
//    ofPopMatrix();
    
    
    // synchronize
    VR::instance()->lock();
    //--------------------------------------------------------------------------
    // draw ChucK-controlled elements
    //--------------------------------------------------------------------------
    // render light
    if( VR::instance()->lightSwitch() ) ofEnableLighting();
    else ofDisableLighting();
    
    // update it
    vr->root()->renderAll();
    
    // stop rendering light
    ofDisableLighting();
    
    // sychronize
    VR::instance()->release();

    //--------------------------------------------------------------------------
    // billboard and draw the mouse
    //--------------------------------------------------------------------------
    if(oculusRift.isSetup())
    {
        ofPushMatrix();
        oculusRift.multBillboardMatrix();
        ofSetColor(255, 0, 0);
        ofCircle(0,0,.5);
        ofPopMatrix();
    }
}


//------------------------------------------------------------------------------
// name: audioIn()
// desc: audio input callback
//------------------------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels)
{
    assert( bufferSize == MY_BUFFERSIZE );
    assert( nChannels == MY_CHANNELS_IN );
    chuck->onInput( input, bufferSize );
    
}


//------------------------------------------------------------------------------
// name: audioOut()
// desc: audio output callback
//------------------------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels)
{
    assert( bufferSize == MY_BUFFERSIZE );
    assert( nChannels == MY_CHANNELS_OUT );
    // chuck
    chuck->onOutput( output, bufferSize );
}


//------------------------------------------------------------------------------
// name: keyPressed()
// desc: on key pressed
//------------------------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    // dismiss safety warning on any key
    oculusRift.dismissSafetyWarning();
    
    if( key == 'f' )
    {
        //gotta toggle full screen for it to be right
        ofToggleFullscreen();
    }
    
    
    if(key == 'a'){
        cout << "FPS " << ofGetFrameRate() << " TARGET " << ofGetTargetFrameRate() << endl;
    }
    
    
    if(key == 's'){
        oculusRift.reloadShader();
    }
    if(key == 'v'){
        oculusRift.setVignette( !oculusRift.getVignette() );
    }
    
    if(key == 'l'){
        oculusRift.lockView = !oculusRift.lockView;
    }
    
    if(key == 'o'){
        showOverlay = !showOverlay;
    }
    if(key == 'r'){
        oculusRift.reset();
    }
    if(key == 'c'){
        oculusRift.recenterPose();
    }
    if(key == 'h'){
        ofHideCursor();
    }
    if(key == 'H'){
        ofShowCursor();
    }
    
    if(key == 'd'){
        oculusRift.setPixelDensity( oculusRift.getPixelDensity()-0.1 );
    }
    if(key == 'D'){
        oculusRift.setPixelDensity( oculusRift.getPixelDensity()+0.1 );
    }
    
    if(key == 'p'){
        oculusRift.setPositionTracking( !oculusRift.getPositionTracking() );
    }
}


//------------------------------------------------------------------------------
// name: keyReleased()
// desc: on key released
//------------------------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//------------------------------------------------------------------------------
// name: mouseMoved()
// desc: on mouse moved
//------------------------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{
    chuck->db()->setFloat( "mouse-x", x );
    chuck->db()->setFloat( "mouse-y", y );
}

//------------------------------------------------------------------------------
// name: mouseDragged()
// desc: on mouse dragged
//------------------------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//------------------------------------------------------------------------------
// name: mousePressed()
// desc: on mouse pressed
//------------------------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//------------------------------------------------------------------------------
// name: mouseReleased()
// desc: on mouse released
//------------------------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//------------------------------------------------------------------------------
// name: mouseEntered()
// desc: on mouse entered
//------------------------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//------------------------------------------------------------------------------
// name: mouseExited()
// desc: on mouse exited
//------------------------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//------------------------------------------------------------------------------
// name: windowResized()
// desc: on window resized
//------------------------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    // log
    cerr << "window resized w: " << w << " h: " << h << endl;
}

//------------------------------------------------------------------------------
// name: gotMessage()
// desc: on receive of message
//------------------------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//------------------------------------------------------------------------------
// name: dragEvent()
// desc: on drag event
//------------------------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
