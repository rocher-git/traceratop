var ZERO   = vec3.fromValues(0, 0, 0);
var AXIS_X = vec3.fromValues(1, 0, 0);
var AXIS_Y = vec3.fromValues(0, 1, 0);
var AXIS_Z = vec3.fromValues(0, 0,-1);

function Camera(canvas, onCameraChanged) {

	this.canvas = canvas;

	// Camera vectors.
	this.ey = vec3.fromValues(0, 0, 0);
	this.at = vec3.fromValues(0, 0, 0);
	this.up = vec3.fromValues(0, 0, 0);
	this.fw = vec3.create();
	this.lf = vec3.create();
	this.worldUp = vec3.fromValues(0,0,0);

	// Mouse navigation.
	this.moving = false;
	this.movingIniMouse = vec2.create();
	this.movingIniCameraOffset = vec3.fromValues(0,0.5,-1);
	this.movingCurCameraOffset = vec3.fromValues(0,0.5,-1);

	// Zoom.
	this.minZoom = 1;
	this.zoom = 2;
	this.maxZoom = 7;
	
	// Mouse rotation.
	this.rotating = false;
	this.rotatingArcBallScale = 0.8;
	this.rotatingArcBallRatio = 0.0;

	this.rotatingArcBallIniPosition = vec3.create();
	this.rotatingArcBallCurPosition = vec3.create();
	this.rotatingIniAngles = vec3.fromValues(0,0,0);
	this.rotatingCurAngles = vec3.fromValues(0,0,0);

	// Temp values.
	this.tmpCameraEy = vec3.fromValues(0,0,0);
	this.tmpCameraAt = vec3.fromValues(0,0,0);
	this.tmpCameraUp = vec3.fromValues(0,0,0);
	this.tmpCameraFw = vec3.fromValues(0,0,0);
	this.tmpIniArcBallPosition = vec3.fromValues(0,0,0);
	this.tmpCurArcBallPosition = vec3.fromValues(0,0,0);

	this.tmpQuatAxisX = quat.create();
	this.tmpQuatAxisY = quat.create();
	this.tmpQuatAxisZ = quat.create();

	this.canvas.addEventListener('mousedown', this.onMouseDown.bind(this));
	this.canvas.addEventListener('mouseup', this.onMouseUp.bind(this));
	this.canvas.addEventListener('mousemove', this.onMouseMove.bind(this));
	this.canvas.addEventListener('mousewheel', this.onMouseWheel.bind(this));
	this.canvas.addEventListener('contextmenu', function(event) {event.preventDefault(); return false});
	
	this.layout();
	
	updateCamera(this);
	this.onCameraChanged = onCameraChanged;
}

var proto = Camera.prototype;

proto.layout = function() {

	var w = this.canvas.clientWidth;
	var h = this.canvas.clientHeight;

	// Update rotation information.
	this.rotatingArcBallRatio = (w >= h ? h : w) * this.rotatingArcBallScale * 0.5;
};

proto.onMouseDown = function(event) {

	event.preventDefault();

	var rect = this.canvas.getBoundingClientRect();
	var x = (event.clientX - rect.left);
	var y = (event.clientY - rect.top);

	this.moving = event.button === 2 || (event.which == 3) || (event.metaKey || event.altKey ||event.ctrlKey);
	this.rotating = event.button === 0 && !this.moving;

	if (this.moving) {
		var iniMouse = this.movingIniMouse;

		iniMouse[0] = x;
		iniMouse[1] = y;
	}

	if (this.rotating) {
		clientToArcBall(this, x, y, this.rotatingArcBallIniPosition);
	}
};

proto.onMouseUp = function(event) {

	event.preventDefault();

	this.moving = false;
	vec3.copy(this.movingIniCameraOffset, this.movingCurCameraOffset);

	this.rotating = false;
	vec3.copy(this.rotatingIniAngles, this.rotatingCurAngles);
};

proto.onMouseMove = function(event) {

	event.preventDefault();

	if(!this.moving && !this.rotating) {
		return;
	}

	var rect = this.canvas.getBoundingClientRect();
	var x = (event.clientX - rect.left) ;
	var y = (event.clientY - rect.top);

	if (this.moving) {
		updateMove(this, x, y);
	}

	if (this.rotating) {
        updateRotation(this, x, y);
	}

	updateCamera(this);
};

proto.onMouseWheel = function(event) {

	event.preventDefault();
	updateZoom(this);
};

function setLookAt(self, ey, at, up) {

	vec3.copy(self.ey, ey);
	vec3.copy(self.at, at);
	vec3.copy(self.up, up);

	vec3.subtract(self.fw, at, ey);
	vec3.normalize(self.fw, self.fw);
	vec3.cross(self.lf, self.fw, up);
	vec3.cross(self.worldUp, self.lf, self.fw);
	vec3.cross(self.lf, self.fw, self.worldUp);
	
	self.onCameraChanged && self.onCameraChanged();
}
	
function updateMove(self, x, y) {

	var dx = 4 * (x - self.movingIniMouse[0]);
	var dy = 4 * (y - self.movingIniMouse[1]);

	var up = self.up;
	var lf = self.lf;

	var iniOffset = self.movingIniCameraOffset;
	var curOffset = self.movingCurCameraOffset;

	var scale = self.zoom * 0.0008;

	curOffset[0] = iniOffset[0]  + (-lf[0] * dx + up[0] * dy) * scale;
	curOffset[1] = iniOffset[1]  + (-lf[1] * dx + up[1] * dy) * scale;
	curOffset[2] = iniOffset[2]  + (-lf[2] * dx + up[2] * dy) * scale;
}

function updateZoom(self) {

	self.zoom -= event.wheelDelta / 200;
	self.zoom = Math.max(Math.min(self.maxZoom, self.zoom), self.minZoom);

	var ey = self.tmpCameraEy;
	var at = self.tmpCameraAt;
	var bk = self.tmpCameraFw;

	vec4.copy(ey, self.ey);
	vec4.copy(at, self.at);
	vec4.copy(bk, self.fw);
	vec4.negate(bk, bk);

	var zoom = self.zoom;

	ey[0] = at[0] + bk[0] * zoom;
	ey[1] = at[1] + bk[1] * zoom;
	ey[2] = at[2] + bk[2] * zoom;

	setLookAt(self, ey, at, self.up);
}

function updateRotation(self, x, y) {

	var iniArcBallPosition = self.rotatingArcBallIniPosition;
	var curArcBallPosition = self.rotatingArcBallCurPosition;
	var tmpIniPosition = self.tmpIniArcBallPosition;
	var tmpCurPosition = self.tmpCurArcBallPosition;

	var dot;
	if(!clientToArcBall(self, x, y,  curArcBallPosition)) {
		dot = vec3.dot(curArcBallPosition, iniArcBallPosition);
		dot = dot > 1 ? 1 : dot;

		if (vec3.cross(tmpIniPosition, iniArcBallPosition, curArcBallPosition)[2] < 0) {
			self.rotatingCurAngles[2] = self.rotatingIniAngles[2] - Math.acos(dot);
		} else {
			self.rotatingCurAngles[2] = self.rotatingIniAngles[2] + Math.acos(dot);
		}

		self.rotatingCurAngles[0] = self.rotatingIniAngles[0];
		self.rotatingCurAngles[1] = self.rotatingIniAngles[1];
	} else {

		vec3.copy(tmpIniPosition, iniArcBallPosition);
		vec3.copy(tmpCurPosition, curArcBallPosition);

		tmpIniPosition[0] = 0;
		tmpCurPosition[0] = 0;

		vec3.normalize(tmpIniPosition, tmpIniPosition);
		vec3.normalize(tmpCurPosition, tmpCurPosition);

		dot = vec3.dot(tmpIniPosition, tmpCurPosition);
		dot = dot > 1 ? 1 : dot;

		if (tmpIniPosition[1] > tmpCurPosition[1]) {
			self.rotatingCurAngles[0] = self.rotatingIniAngles[0] - Math.acos(dot);
		} else {
			self.rotatingCurAngles[0] = self.rotatingIniAngles[0] + Math.acos(dot);
		}

		vec3.copy(tmpIniPosition, iniArcBallPosition);
		vec3.copy(tmpCurPosition, curArcBallPosition);

		tmpIniPosition[1] = 0;
		tmpCurPosition[1] = 0;

		vec3.normalize(tmpIniPosition, tmpIniPosition);
		vec3.normalize(tmpCurPosition, tmpCurPosition);

		dot = vec3.dot(tmpIniPosition, tmpCurPosition);
		dot = dot > 1 ? 1 : dot;

		if (tmpIniPosition[0] < tmpCurPosition[0]) {
			self.rotatingCurAngles[1] = self.rotatingIniAngles[1] - Math.acos(dot);
		} else {
			self.rotatingCurAngles[1] = self.rotatingIniAngles[1] + Math.acos(dot);
		}

		self.rotatingCurAngles[2] = self.rotatingIniAngles[2];
	}
}

function clientToArcBall(self, x, y, position) {

	var w = self.canvas.clientWidth;
	var h = self.canvas.clientHeight;

	position[0] = (x - w * 0.5) / self.rotatingArcBallRatio;
	position[1] = (y - h * 0.5) / self.rotatingArcBallRatio;

	var radiusSquared = (position[0] * position[0]) + (position[1] * position[1]);
	var invRadius;

	if (radiusSquared <= 1 ) {
		position[2] = 1.0 - Math.sqrt(radiusSquared);
	} else {
		invRadius = 1.0 / Math.sqrt(radiusSquared);
		position[0] *= invRadius;
		position[1] *= invRadius;
		position[2]  = 0;
	}

	return radiusSquared <= 1.0;
}

function updateCamera(self) {

	// Initialize camera.
	var ey = self.tmpCameraEy;
	var at = self.tmpCameraAt;
	var up = self.tmpCameraUp;

	vec3.copy(ey, AXIS_Z);
	vec3.copy(at, ZERO);
	vec3.copy(up, AXIS_Y);

	// Update camera.
	var rotationX = self.tmpQuatAxisX;
	var rotationY = self.tmpQuatAxisY;
	var rotationZ = self.tmpQuatAxisZ;

	quat.setAxisAngle(rotationX, AXIS_X, self.rotatingCurAngles[0]);
	quat.setAxisAngle(rotationY, AXIS_Y, self.rotatingCurAngles[1]);
	quat.setAxisAngle(rotationZ, AXIS_Z, self.rotatingCurAngles[2]);

	quat.multiply(rotationX, rotationX, rotationZ);
	quat.multiply(rotationZ, rotationY, rotationX);

	vec3.transformQuat(ey, ey, rotationZ);
	vec3.transformQuat(up, up, rotationZ);

	// Apply zoom.
	var zoom = self.zoom;

	ey[0] *= zoom;
	ey[1] *= zoom;
	ey[2] *= zoom;

	// Update position.
	var offset = self.movingCurCameraOffset;

	ey[0] += offset[0];
	ey[1] += offset[1];
	ey[2] += offset[2];

	at[0] += offset[0];
	at[1] += offset[1];
	at[2] += offset[2];

	setLookAt(self, ey, at, up);
}