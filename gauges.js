    // Gavin Roake KBR
    // 7/16/2024
    // Custom Gauge Cluster

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
    
    
    

    

    //Periodically update gauges
    setInterval( () => {
        updateTemp(Math.random() + 81.8);

        attitude.setRoll(Math.random()*2);
        attitude.setPitch(Math.random()*0.5);

        airspeed.setAirSpeed(Math.random()*5+100);

        altimeter.setAltitude(Math.random()*20+3000);
        altimeter.setPressure(0,0);

        heading.setHeading(Math.random()*5);

        turncoordinator.setTurn(Math.random()*10 - 10);
        turncoordinator.setSlip(Math.random()*2 - 0.5);
        turncoordinator.setHeading(300);

        variometer.setVario(Math.random()*2);
    },100 );
