import asyncio

class PeriodicTask:
    def __init__(self, behaviour, period):
        self.behaviour = behaviour
        self.period = period
        self._is_started = False

    async def start(self):
        if not self._is_started:
            self._is_started = True
            # Start task to call func periodically:
            await self._run()

    async def _run(self):
        while True:
            await self.behaviour()
            await asyncio.sleep(self.period)