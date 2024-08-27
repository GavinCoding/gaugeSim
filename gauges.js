// Gavin Roake KBR
// 7/16/2024
// Custom Gauge Cluster



//WebSocket/Server stuff
const socket =  new WebSocket("ws://localhost:8081");

socket.onopen = function(event){
    console.log("Websocket connection est");
};

 


//should take in message sort which gauge the data is for then update the gauge with the prorper function
socket.onmessage = function(event){
    
    var numericData = parseFloat(event.data);
    console.log("data recieved from server:", event.data);
    updateTemp(numericData);
};

socket.onmessage = function(event) {
    // Split the received data into type and value
    var data = event.data;
    var dataType = data.charAt(0); // First character is the data type
    var value = data.substring(2); // Remaining part is the value

    // Convert value to a number if possible
    var numericValue = parseFloat(value);

    console.log("Data received from server:", data);

    // Switch case to handle different types of data
    switch (dataType) {
        case 'T': // Temperature
            updateTemp(numericValue);
            break;
        case 'R': // Roll
            attitude.setRoll(numericValue);
            break;
        case 'P': // Pitch
            attitude.setPitch(numericValue);
            break;
        case 'A': // AirSpeed
            airspeed.setAirSpeed(numericValue);
            break;
        case 'L': // Altitude
            altimeter.setAltitude(numericValue);
            break;
        case 'U': //Pressure
            altimeter.setPressure(numericValue,0);
            break;
        case 'H': // Heading
            heading.setHeading(numericValue);
            break;
        case 'N': // Turn
            turncoordinator.setTurn(numericValue);
            break;
        case 'S': // Slip
            turncoordinator.setSlip(numericValue);
            break;
        case 'V': //Vario
            variometer.setVario(numericValue);
            break;
        default:
            console.error("Unknown data type:", dataType);
    }
};






socket.onclose = function(event){
    console.log("server closed");
}

socket.onerror = function(error){
    console.log("webSocket error:", error)
};

//Temperture Gauge setters
function updateTemp(temp){
    document.getElementById('tempDisplay').innerText = temp.toFixed(1);
}




//Skyhawk gauge implementation(see HTML for more details)
var options = {
    size : 225,				// Sets the size in pixels of the indicator (square)
    showBox : false,			// Sets the visibility of the box behind the instruments
    showScrews: false,		// Sets the visibility of the four screws around the instruments
    img_directory : 'img/'	// The directory where the images are saved to
}

var attitude = $.flightIndicator('#attitude', 'attitude',options);
var airspeed = $.flightIndicator('#airspeed', 'airspeed',options);
var altimeter = $.flightIndicator('#altimeter', 'altimeter', options);
var turncoordinator = $.flightIndicator('#turn_coordinator', 'turn_coordinator', options);
var heading = $.flightIndicator('#heading ', 'heading', options);
var variometer= $.flightIndicator('#variometer', 'variometer', options);



