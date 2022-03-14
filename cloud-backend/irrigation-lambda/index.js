const AWS = require('aws-sdk')
const publishMessage = require('aws-mqtt/lib/publishMessage')

const IRRIGATION_COMMAND_TOPIC = "irrigation/command";

const MS_PER_MINUTE = 60000;
const DURATION_IN_MINUTES = 10;
const DURANTION_IN_MILLIS = DURATION_IN_MINUTES * MS_PER_MINUTE;
    
AWS.config.region = 'us-east-1';

const brokerConfig = {
    region: AWS.config.region,
    credentials: AWS.config.credentials,
    endpoint: 'a2vlodv4jxozlb-ats.iot.us-east-1.amazonaws.com'
};

const docClient = new AWS.DynamoDB.DocumentClient();

function readData() {
    const params = {
        TableName: "measurements",
        ProjectionExpression: "*",
        FilterExpression: "#tm >= :timestamp",
        ExpressionAttributeNames: {
            "#tm": "timestamp",
        },
        ExpressionAttributeValues: {
             ":timestamp": new Date(Date.now() - DURANTION_IN_MILLIS) 
        }
    };

    return docClient.scan(params).promise();
}

function average(arr) {
    return arr.reduce((first, second) => first + second, 0) / arr.length;
}

function aggregate_measurements(data) {
    let moisture = [];
    let temperature = [];
    let humidity = [];
    let light = [];
    let rain = [];
    
    data.Items.map(payload => payload.Payload).forEach(measurement => {
        if ('soil-moisture' in measurement) {
            moisture.push(measurement['soil-moisture']);
        }

        if ('temperature' in measurement) {
            temperature.push(measurement.temperature);
        }

        if ('humidity' in measurement) {
            humidity.push(measurement.humidity);
        }

        if ('light-intensity' in measurement) {
            light.push(measurement['light-intensity']);
        }

        if ('rain' in measurement) {
            rain.push(measurement.rain);
        }
    });
    
    return {
        soil_moisture: average(moisture),
        temperature: average(temperature),
        humidity: average(humidity),
        light_intensity: average(light),
        rain: average(rain)
    };
}

async function irrigation_decision(measurement) {
    if (measurement.soil_moisture < 80) {
        // Dry soil causes dehydration of crops
        if (measurement.rain < 1) {
            // It only irrigates if it does not rain, in order to save water.
            if (measurement.temperature < 25 && measurement.humidity < 40) {
                /* Only in good environmental conditions can it irrigate:
                 *  - the temperature is not high, thus avoiding the evaporation of water
                 *  - the percentage of humidity is low.
                 */
                 if (measurement.light_intensity < 66000) {
                     return "on";
                 }
            }
        }
    } 
    
    return "off";
}

exports.handler = async () => {
    try {
        let data = await readData();
        let m = aggregate_measurements(data);
        console.log("Average measurement: ", m);

        let irrigation_command = await irrigation_decision(m);
        
        let command = JSON.stringify({
            irrigation: irrigation_command
        });

        console.log("Irrigation:", command);
        if (irrigation_command == "on") {
            await publishMessage(brokerConfig, IRRIGATION_COMMAND_TOPIC, command);
        }
    } catch(e) {
        console.log("Connection closed");
    }
};