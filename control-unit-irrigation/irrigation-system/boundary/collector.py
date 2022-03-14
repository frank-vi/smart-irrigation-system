from boundary.periodic_task import PeriodicTask
import time

class DataCollector(PeriodicTask):
    COLLECT_PERIOD = 20
    
    def __init__(self, ble):
        super().__init__(self.behaviour, self.COLLECT_PERIOD)
        self.ble = ble

    async def start(self):
        await PeriodicTask.start(self)

    async def behaviour(self):
        await self.ble.communicate_with_edge()

