import statistics
from datetime import datetime
from boundary.periodic_task import PeriodicTask

class DataAggregator(PeriodicTask):
    # AGGREGATE_PERIOD = 10 * 60 # 10 minutes
    AGGREGATE_PERIOD = 10 #seconds

    def __init__(self, db, messenger):
        super().__init__(self.behaviour, self.AGGREGATE_PERIOD)
        self.db = db
        self.messenger = messenger

    async def start(self):
        self.db.connect()
        await PeriodicTask.start(self)

    def get_values_of_type(self, measurements, m_type):
        return list(
            map(float, [measurement[1] for measurement in measurements if measurement[0] == m_type]))

    def get_device(self, data):
        return data[0][2]

    def get_timestamp(self):
        now = datetime.now()
        return datetime.timestamp(now)

    def get_battery(self, data):
        battery_levels = self.get_values_of_type(data, "battery")
        battery_levels = [ele for ele in battery_levels if ele != []]
        latest_battery_level = battery_levels[-1] if len(battery_levels) > 1 else battery_levels[0]
        return latest_battery_level

    def aggregate_environmental_measurements(self, data):
        environmental_parameters = ["temperature", "humidity", "rain", "soil-moisture", "light-intensity"]
        measurements = [self.get_values_of_type(data, environmental_parameter) for
                        environmental_parameter in environmental_parameters]

        average_measurements = [round(statistics.mean(valuesOfMeasurement), 2) if valuesOfMeasurement != [] else 0 for
                                valuesOfMeasurement in measurements]
        return list(zip(environmental_parameters, average_measurements))

    def device_info(self, data):
        device_parameters = ["deviceId", "battery", "timestamp"]
        return list(zip(device_parameters, [self.get_device(data), self.get_battery(data), self.get_timestamp()]))

    async def behaviour(self):
        data = self.db.read_data()

        if not data and data is not None:
            print("WARNING: Unable to fetch data. Database is empty")
        else:
            self.db.clear()
            message = dict(self.aggregate_environmental_measurements(data) +
                           self.device_info(data))

            self.messenger.publish_telemetry(message)
