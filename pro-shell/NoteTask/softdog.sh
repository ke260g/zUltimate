
/dev/watchdog

#define SFDOG_ENABLE_CHECK  ("softdog_enable")
#define SFDOG_KEEP_LIVE     ("softdog_keep_live")
#define SFDOG_DISABLE_CHECK ("softdog_disable")

echo softdog_enable > /dev/watchdog
echo softdog_keep_live > /dev/watchdog
echo softdog_disable > /dev/watchdog