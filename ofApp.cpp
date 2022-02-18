#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	// 配色デザイン P092 ビビットイエロー
	this->color_palette.push_back(ofColor(255, 240, 0));
	this->color_palette.push_back(ofColor(245, 217, 51));
	this->color_palette.push_back(ofColor(232, 229, 0));
	this->color_palette.push_back(ofColor(250, 229, 51));
	this->color_palette.push_back(ofColor(231, 180, 33));
	this->color_palette.push_back(ofColor(255, 255, 255));
	this->color_palette.push_back(ofColor(215, 131, 76));
	this->color_palette.push_back(ofColor(138, 196, 221));
}
//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	for (int x = -400; x <= 400; x += 200) {

		for (int y = -400; y <= 400; y += 200) {

			auto location = glm::vec3(x, y, 0);
			for (int radius = 70; radius <= 85; radius += 5) {

				auto noise_location = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
				int face_color_index = ofRandom(this->color_palette.size());
				int frame_color_index = (face_color_index + (int)ofRandom(1, this->color_palette.size())) % this->color_palette.size();

				this->setRingToMesh(this->face, this->frame, location, noise_location, radius, radius / ofRandom(3, 10), this->color_palette[face_color_index], this->color_palette[frame_color_index]);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int x = -400; x <= 400; x += 200) {

		for (int y = -400; y <= 400; y += 200) {

			ofSetColor(this->color_palette[ofRandom(this->color_palette.size())]);
			ofDrawSphere(glm::vec3(x, y, 0), 70);
		}
	}

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, glm::vec3 noise_location, float radius, float height, ofColor face_color, ofColor frame_color) {

	int index = face_target.getNumVertices();

	for (int deg = 0; deg < 360; deg += 1) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 1) * DEG_TO_RAD), radius * sin((deg + 1) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			auto noise_value_x = ofNoise(
				noise_location.x, radius * 0.0008 + ofGetFrameNum() * 0.005);
			auto noise_value_y = ofNoise(
				noise_location.y, radius * 0.0008 + ofGetFrameNum() * 0.005);
			auto noise_value_z = ofNoise(
				noise_location.z, radius * 0.0008 + ofGetFrameNum() * 0.005);

			auto rotation_x = glm::rotate(glm::mat4(), ofMap(noise_value_x, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(noise_value_y, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), ofMap(noise_value_z, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(0, 0, 1));

			vertex = glm::vec4(vertex, 0) * rotation_y * rotation_x + glm::vec4(location, 0);
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}