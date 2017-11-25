/* Daniel Dupriest, 2017
 * Frupal web client javascript
 */

var data; //Javascript object holding all our game data
var tileSize = 20;
var debugMode = false;
var mapDimensions = 25;
var jsonVisible = false;
var commandInProgress = false;
var inputDelay = 50; //in ms
var onObstacle = false; 


var enableDebugMode = function() {
    var elements = document.getElementsByClassName("false");
    for(element in elements)
        element.classList.remove("false");
}

//Button to display the raw json. Hide by default to speed up page reload.
var showJson = function() {
    var rawJsonElement = document.getElementById("rawJson");
    rawJsonElement.innerHTML = "<pre>" + JSON.stringify(data, null, 2) + "</pre>";
}

function sendTool(element){
	console.log(element.innerText);
	
}

function Game() {


    //This function gets called on every key press, and if a valid key is found, sendCommand() is called with the corresponding command.
    this.handleKeys = function(event) {
        if(commandInProgress)
            return;
        commandInProgress = true;
        setTimeout(function(){ commandInProgress = false; }, inputDelay);
        var code = event.keyCode;
        var command;
        console.log("Key pressed");
	onObstacle = data.obstacle;
	if(onObstacle == "true"){
		document.getElementById('toolButtons').style.display = 'block'; 
		document.getElementById("tool1").innerText = data.tool1;
		document.getElementById("tool2").innerText = data.tool2;
		document.getElementById("tool3").innerText = data.tool3;
		document.getElementById("noTool").innerText = data.noTool;
		if(code == 49) 
			command = "tool1";
		else if(code == 50)
			command = "tool2";
		else if(code == 51)
			command = "tool3";
		else if(code == 52)
			command = "noTool";
		else
			return;
	}
	else{
		document.getElementById("toolButtons").style.display = "none";
		if(code == 37) //Left
            		command = "left";
        	else if(code == 38) //Up
           		command = "up";
        	else if(code == 39) //Right
            		command = "right";
        	else if(code == 40) //Down
            		command = "down";
        	else if(code == 32) //Spacebar
           		command = "space";
        	else
            	return;  //Ignore if some other key is pressed
	}
        
        this.sendCommand(command);
    }

    //Redraw all the updatable parts of the screen with new data
    this.redraw = function() {
        console.log("Redrawing.");

		// Display inventory -Austin
		document.getElementById("axe").innerHTML = data.hero.axe;
		document.getElementById("binoculars").innerHTML = data.hero.binoculars ? 1 : 0;
		document.getElementById("boat").innerHTML = data.hero.boat ? 1 : 0;
		document.getElementById("chainsaw").innerHTML = data.hero.chainsaw;
		document.getElementById("chisel").innerHTML = data.hero.chisel;
		document.getElementById("hatchet").innerHTML = data.hero.hatchet;
		document.getElementById("jackhammer").innerHTML = data.hero.jackhammer;
		document.getElementById("machete").innerHTML = data.hero.machete;
		document.getElementById("shears").innerHTML = data.hero.shears;
		document.getElementById("sledge").innerHTML = data.hero.sledge;

        //Print the messages
        var logElement = document.getElementById("log");
        if(data.log)
            logElement.innerHTML = data.log;
	else
	    logElement.innerHTML = "";

        //Print hero position
        var heroPosition = document.getElementById("coordinates");
        heroPosition.innerHTML = "Hero's position is at ( " + data.hero.x + " , " + (mapDimensions - data.hero.y - 1) + " ) "; 

	var heroenergy = document.getElementById("hero_energy");
        heroenergy.innerHTML = "Hero's energy is  " + data.hero.energy;

	if( data.alert )
                  alert( data.alert );
 
        //Redraw the tiles
        var mapElement = document.getElementById("map");
        var toAdd = "";
        var numberOfTiles = data.map.grovnicks.length;
        console.log("Loading " + numberOfTiles + " tiles.");
        for(var i = 0; i < data.map.grovnicks.length; ++i) {
            var tile = data.map.grovnicks[i];
            //Open tile div
            toAdd += "<div class='tile type"
                + tile.terrain + " "
                + (debugMode == false ? tile.visible : "")
                + "' style='left:"
                + (tile.x*tileSize)
                + "px;top:"
                + (tile.y*tileSize)
                + "px'>";
            //Add content div
            toAdd += "<div class='content " + tile.content + "'></div>";
            //Close tile div
            toAdd += "</div>";
        }
        mapElement.innerHTML = toAdd;

        //Redraw the player
        var playerElement = document.getElementById("player");
        playerElement.style.left = data.hero.x * tileSize + "px";
        playerElement.style.top = data.hero.y * tileSize + "px";
        if(jsonVisible) {
            showJson();
        }
    }

    //Send the user's command to the server via ajax call
    this.sendCommand = function(command){
        console.log("Sending comand: " + command);

        var self = this;
        var req = new XMLHttpRequest();
        req.onreadystatechange = function() {
            if(this.readyState == 4 && this.status == 200) {
                data = JSON.parse(req.responseText);
                self.redraw();
            }
        };

        req.open("POST", "frupal.cgi");
        req.setRequestHeader("Content-Type", "application/json");
        req.send(JSON.stringify({"command":command}));
        return false;
    };
    
    this.generateNewMap = function() {
        var req = new XMLHttpRequest();
        var self = this;
        req.onreadystatechange = function() {
            if(this.readyState == 4 && this.status == 500) {
                self.sendCommand("right");
            }
        };
        req.open("GET", "generatePerlinMap.cgi", true);
        req.send();
    };
}
