var noble = require('noble');

noble.on('stateChange', function(state) {
	if(state === 'poweredOn') {
		//console.log('Powered on!');
		noble.startScanning();
	}
});

noble.on('discover', function(device) {
	//console.log('Found device ' + device.address);			
	
	if(device.address === 'c1:44:ea:74:47:42') {
		//console.log(device);
		noble.stopScanning();
		//console.log('Found it!');		
		
		device.connect(function(error) {
			device.discoverAllServicesAndCharacteristics(function(error, services, characteristics) {
					
				var temp_char = null
					
				characteristics.forEach(function(ch, chId) {												
					if(ch.uuid === '2a6e') {
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
