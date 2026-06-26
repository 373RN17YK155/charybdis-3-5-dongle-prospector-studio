zephyr_library()
zephyr_library_sources(monitor.c)

# Include paths for ZMK headers
zephyr_include_directories(
    ${ZMK_BASE}/app/include
)
