set(STATES_SOURCES
    "states/state.cpp"
    "states/state_manager.cpp"
    "states/state_subsystem.cpp"
    "states/connectivity/connected_state.cpp"
    "states/connectivity/connection_test_state.cpp"
    "states/connectivity/provisioning_state.cpp"
    "states/ui/ui_provisioning_input_state.cpp"
    "states/ui/ui_connection_test_state.cpp"
    "states/ui/ui_connected_state.cpp"
    "states/ui/ui_main_screen_state.cpp"
    "states/ui/ui_prediction_state.cpp"
    )

list(APPEND SOURCES ${STATES_SOURCES})