/**
 * o------------------------------------------------------------------------------o
 * | This package is licensed under the Phpguru license. A quick summary is       |
 * | that for commercial use, there is a small one-time licensing fee to pay. For |
 * | registered charities and educational institutes there is a reduced license   |
 * | fee available. You can read more  at:                                        |
 * |                                                                              |
 * |                  http://www.phpguru.org/static/license.html                  |
 * o------------------------------------------------------------------------------o
 */
function Clock (id)
{
	this.id      = id;
	this.canvas  = document.getElementById(id);
	this.context = this.canvas.getContext('2d');
	this.canvas.__object__ = this;
	
	this.centerx = this.canvas.width / 2;
	this.centery = this.canvas.height / 2;
	
	this.properties = [];
	this.properties['clock.strokestyle']   = 'black';
	this.properties['clock.gutter']        = 25;
	this.properties['clock.shadow']        = true;
	this.properties['clock.numbers']       = true;
	this.properties['clock.background']    = true;
	this.properties['clock.digital']       = true;
	this.properties['clock.text.size']     = 12;
	this.properties['clock.numbers.style'] = 'numbers';
	this.properties['clock.centerpin']     = true;
	this.properties['clock.hands.tails']   = true;
	this.properties['clock.numbers.style.lines.linewidth'] = 13;
	
	this.radius  = Math.min(this.canvas.width / 2, this.canvas.height / 2) - this.Get('clock.gutter');
}


/**
 * A setter
 * 
 * @param name  string The name of the property to set
 * @param value mixed  The value of the property
 */
Clock.prototype.Set = function (name, value)
{
	this.properties[name.toLowerCase()] = value;
}


/**
 * A getter
 * 
 * @param name  string The name of the property to get
 */
Clock.prototype.Get = function (name)
{
	return this.properties[name.toLowerCase()];
}


/**
 * Progressively draws the clock
 */
Clock.prototype.Draw = function ()
{
	// First, clear the canvas
	this.canvas.width = this.canvas.width;
	
	this.Drawface();
	this.Drawnumbers();
	this.DrawCentre();
	this.Drawsecond();
	this.Drawminute();
	this.Drawhour();
	this.DrawDigital();
}


/**
 * Draws the face of the clock
 */
Clock.prototype.Drawface = function ()
{
	/**
	 * Draw the background.face at all?
	 */
	if (!this.Get('clock.background')) {
		return;
	}
	
	/**
	 * If a shadow is requested, turn on the shadow, draw a circle, and then turn it off again
	 */
	if (this.Get('clock.shadow')) {
		
		this.context.lineWidth = 1;
		this.context.strokeStyle = this.Get('clock.strokestyle');
		this.context.fillStyle = 'white';
		this.context.beginPath();
		
		this.context.shadowColor = 'rgba(0,0,0,0.5)';
		this.context.shadowOffsetX = 3;
		this.context.shadowOffsetY = 3;
		this.context.shadowBlur = 3;
		
		this.context.beginPath();
		this.context.arc(this.centerx, this.centery, this.radius, 0, 6.28, 0);
		
		this.context.stroke();
		this.context.fill();
		
		// Now turn it off
		this.context.shadowColor = 'rgba(0,0,0,0)';
		this.context.shadowOffsetX = 0;
		this.context.shadowOffsetY = 0;
		this.context.shadowBlur = 0;
	}
	
	// Draw the main circle
	this.context.beginPath();
	this.context.strokeStyle = this.Get('clock.strokestyle');
	this.context.arc(this.centerx, this.centery, this.radius, 0, 6.28, 0);
	this.context.stroke();
	
	// Now draw the small ticks
	this.context.beginPath();
	for (var i=0; i<360; i+=10) {
		this.context.arc(this.centerx, this.centery, this.radius, (i+0.1)/57.3, i / 57.3, 0);
		this.context.lineTo(this.centerx, this.centery);
	}
	this.context.stroke();
	
	// Now draw a big white circle to create the tick marks
	this.context.fillStyle = 'white';
	this.context.strokeStyle = 'white';
	this.context.beginPath();
	this.context.arc(this.centerx, this.centery, this.radius - 3, 0, 6.28, 0);
	this.context.stroke();
	this.context.fill();
}


/**
 * Draw the numbers on the clock
 */
Clock.prototype.Drawnumbers = function ()
{
	if (!this.Get('clock.numbers')) {
		return;
	}
	
	if (this.Get('clock.numbers.style') == 'numbers') {
		var r = this.radius - 10;
		
		this.Drawtext('1', this.centerx + (0.5 * r), this.centery - (0.88 * r), this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('2', this.centerx + (0.866 * r), this.centery - (0.5 * r), this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('3', this.centerx + this.radius - 10, this.centery, this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('4', this.centerx + (0.866 * r), this.centery + (0.5 * r), this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('5', this.centerx + (0.5 * r), this.centery + (0.866 * r), this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('6', this.centerx, this.centery + r, this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('7', this.centerx - (0.5 * r), this.centery + (0.866 * r), this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('8', this.centerx - (0.866 * r), this.centery + (0.49 * r), this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('9', this.centerx - this.radius + 10, this.centery, this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('10',this.centerx - (0.866 * r),this.centery - (0.50 * r),this.Get('clock.text.size'),'center','center');
		this.Drawtext('11', this.centerx - (0.51 * r), this.centery - (0.88 * r), this.Get('clock.text.size'), 'center', 'center');
		this.Drawtext('12', this.centerx, 35, this.Get('clock.text.size'), 'center', 'center');
		
	} else {
		
		this.context.strokeStyle = 'black';
		this.context.lineWidth = this.Get('clock.numbers.style.lines.linewidth');
		
		for (var a=0; a<360; a+=30) {
			this.context.beginPath();
			this.context.arc(this.centerx, this.centery, this.radius - 5, a / 57.3, a / 57.3, 0);
			this.context.arc(this.centerx, this.centery, this.radius - 5 - (this.radius * 0.25), a / 57.3, a / 57.3, 0);
			this.context.stroke();
		}
		
		
		this.context.strokeStyle = 'white';
		this.context.lineWidth = 3;
		
		for (var a=0; a<360; a+=30) {
			this.context.beginPath();
			this.context.arc(this.centerx, this.centery, this.radius - 5, a / 57.3, a / 57.3, 0);
			this.context.arc(this.centerx, this.centery, this.radius - 35, a / 57.3, a / 57.3, 0);
			this.context.stroke();
		}
	}
}


/**
 * A function which draws some text
 */
Clock.prototype.Drawtext = function (text, x, y, size)
{
	this.context.font = size + 'pt Verdana';
	this.context.fillStyle = this.Get('clock.strokestyle');
	var halign = arguments[4] ? arguments[4] : 'left';
	var valign = arguments[5] ? arguments[5] : 'bottom';
	
	// halign
	if (halign == 'right') {
		x -= this.context.measureText(text).width;
	} else if (halign == 'center') {
		x -= (this.context.measureText(text).width / 2);
	}
	
	// valign
	if (valign == 'top') {
		y += size;
	} else if (valign == 'center') {
		y += (size / 2);
	}
	
	this.context.save();
	this.context.beginPath();
	this.context.translate(x, y);
	this.context.fillText(text,0,0);
	
	this.context.stroke();
	this.context.fill();
	
	this.context.restore();
}


/**
 * Draws the second hand
 */
Clock.prototype.Drawsecond = function ()
{
	var date = new Date();
	var s = date.getSeconds();
	
	this.context.lineWidth   = 2;
	this.context.lineCap     = 'round';
	this.context.strokeStyle = this.Get('clock.strokestyle');
	
	this.context.beginPath();
	this.context.arc(this.centerx, this.centery, this.radius - 30, ((s/60) * 6.27) - 1.57, ((s/60) * 6.28) - 1.57, 0);
	this.context.lineTo(this.centerx, this.centery);
	
	if (this.Get('clock.hands.tails')) {
		this.context.arc(this.centerx, this.centery, 15, ((s/60) * 6.28) + 1.57, ((s/60) * 6.28) + 1.57, 0);
		this.context.lineTo(this.centerx, this.centery);
	}
	
	this.context.stroke();
}


/**
 * Draws the minute hand
 */
Clock.prototype.Drawminute = function ()
{
	var date = new Date();
	var m = date.getMinutes();
	
	this.context.strokeStyle = this.Get('clock.strokestyle');
	this.context.lineWidth   = 5;
	this.context.lineCap     = 'round';
	
	this.context.beginPath();
	this.context.arc(this.centerx, this.centery, this.radius - 40, ((m/60) * 6.27) - 1.57, ((m/60) * 6.28) - 1.57, 0);
	this.context.lineTo(this.centerx, this.centery);
	
	if (this.Get('clock.hands.tails')) {
		this.context.arc(this.centerx, this.centery, 15, ((m/60) * 6.28) + 1.57, ((m/60) * 6.28) + 1.57, 0);
		this.context.lineTo(this.centerx, this.centery);
	}
	
	this.context.stroke();
}


/**
 * Draws the hour hand
 */
Clock.prototype.Drawhour = function ()
{
	var date = new Date();
	var h = date.getHours();
	var m = date.getMinutes();
	
	/**
	 * Account for 24h time
	 */
	if (h >= 12) {
		h -= 12;
	}
	
	this.context.strokeStyle = this.Get('clock.strokestyle');
	this.context.lineWidth   = 5;
	this.context.lineCap     = 'round';
	
	var a = ((h/12) * 6.28) - 1.57 + ((m / 60) * 0.524);
	
	this.context.beginPath();
	this.context.arc(this.centerx, this.centery, this.radius - 65, a + 0.01, a, 1);
	this.context.lineTo(this.centerx, this.centery);
	
	if (this.Get('clock.hands.tails')) {
		this.context.arc(this.centerx, this.centery, 15, ((h/12) * 6.27) + 1.57, ((h/12) * 6.28) + 1.57, 0);
		this.context.lineTo(this.centerx, this.centery);
	}
	
	this.context.stroke();
}

/**
 * Draws the digital readout at the bottom of the clock
 */
Clock.prototype.DrawDigital = function ()
{
	if (!this.Get('clock.digital')) {
		return;
	}
	
	/**
	 * Now draw the digital readout at the bottom
	 */
	var date = new Date();
	var hours   = String(date.getHours());
	var minutes = String(date.getMinutes());
	var seconds = String(date.getSeconds());
	
	
	// Now pad them to two chars minimum
	if (hours.length == 1)   hours   = '0' + hours;
	if (minutes.length == 1) minutes = '0' + minutes;
	if (seconds.length == 1) seconds = '0' + seconds;
	
	var str = hours + ':' + minutes + ':' + seconds;
	this.Drawtext(str, this.centerx, this.centery + this.radius + 12, 14, 'center', 'center');
}


/**
 * This draws the centre bit of the clock
 */
Clock.prototype.DrawCentre = function ()
{
	if (this.Get('clock.centerpin') == false) {
		return;
	}
	
	this.context.strokeStyle = 'rgba(0,0,0,0)';
	
	/**
	 * Draw the small centre bit
	 */
	this.context.beginPath();
	this.context.fillStyle = this.Get('clock.strokestyle');
	this.context.beginPath();
	this.context.arc(this.centerx, this.centery, 10, 0, 6.28, 0);
	this.context.stroke();
	this.context.fill();
	
	/**
	 * Now draw a semi-opaque cicle over the circle we've just drawn to darken it
	 */
	this.context.beginPath();
	this.context.fillStyle = 'rgba(0,0,0,0.2)';
	this.context.beginPath();
	this.context.arc(this.centerx, this.centery, 10, 0, 6.28, 0);
	this.context.stroke();
	this.context.fill();
}