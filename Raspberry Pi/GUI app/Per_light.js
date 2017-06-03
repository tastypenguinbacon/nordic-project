var noble = require('noble');

var param = null

noble.on('stateChange', function(state) {
	if(state === 'poweredOn') {
		//console.log('Powered on!');
		noble.startScanning();
	}
	if (process.argv.length <= 2) {
    console.log("Usage: " + __filename + " SOME_PARAM");
    process.exit(-1);
	}
	param = process.argv[2];
});

noble.on('discover', function(device) {
	//console.log('Found device ' + device.address);			
	
	if(device.address === param) {
		//console.log(device);
		noble.stopScanning();
		//console.log('Found it!');		
		
		device.connect(function(error) {
			device.discoverAllServicesAndCharacteristics(function(error, services, characteristics) {
					
				var temp_char = null
					
				characteristics.forEach(function(ch, chId) {												
					if(ch.uuid === '835d') {
						//console.log('found char');
						temp_char = ch;
					}
				});			

				temp_char.read(function(error, data) {					
					console.log(data.readUInt8(0));
					if (error) {
						//console.log('error!!111!!');
					}	
					device.disconnect(function(error) {
						process.exit(0);
					});
				});		
							
			});	
		});
	}
});
