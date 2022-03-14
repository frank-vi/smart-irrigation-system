"""Main script for Surveillance Telegram Bot."""
import logging
import os

from surveillance_bot import bot

# Configuration environment variables.
AUTHORIZED_USER1 =  "MarcusPan"
AUTHORIZED_USER2 =  "frank_vi"
BOT_API_TOKEN = "1408403839:AAHNIdiGAvCaCOrT4lZMGGJyCsEVvgZFUbM"
PERSISTENCE_DIR = os.environ.get('PERSISTENCE_DIR', None)
LOG_LEVEL = os.environ.get('LOG_LEVEL', logging.WARNING)
BOT_LOG_LEVEL = os.environ.get('BOT_LOG_LEVEL', None)


# Logging config.
logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=LOG_LEVEL
)
logging.captureWarnings(True)
logging.getLogger('py.warnings').setLevel(logging.ERROR)


def main() -> None:
    """Surveillance Telegram Bot start up function."""
    surveillance_bot = bot.Bot(
        token=BOT_API_TOKEN,
        usernames=[AUTHORIZED_USER1, AUTHORIZED_USER2],
        persistence_dir=PERSISTENCE_DIR,
        log_level=BOT_LOG_LEVEL
    )
    surveillance_bot.start()


if __name__ == '__main__':  # pragma: no cover
    main()
