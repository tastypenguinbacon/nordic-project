var noble = require('noble');

var param = null

noble.on('stateChange', function(state) {
	if(state === 'poweredOn') {
		//console.log('Powered on!');
		noble.startScanning();
	}
	if (process.argv.length <= 3) {
    console.log("Usage: " + __filename + " SOME_PARAM");
    process.exit(-1);
	}
	param = process.argv[2];
	value = process.argv[3];
});

noble.on('discover', function(device) {
	//console.log('Found device ' + device.address);			
	
	if(device.address === param) {
		//console.log(device);
		noble.stopScanning();
		//console.log('Found it!');		
		
		device.connect(function(error) {
			device.discoverAllServicesAndCharacteristics(function(error, services, characteristics) {
					
				var act_char = null
					
				characteristics.forEach(function(ch, chId) {												
					if(ch.uuid === '9871') {
						//console.log('found char');
						act_char = ch;
					}
				});			

				act_char.write(new Buffer([value]), true, function(error) {
					if (error) {
						//console.log('error!!111!!');
					}	
					console.log('written');
					device.disconnect(function(error) {
						process.exit(0);
					});
				});		
							
			});	
		});
	}
});
