// #define WIFI_SSID "THE BRACELET"
// #define WIFI_PASS "2010#PepsiHTH"
//  #define WIFI_SSID "Aghiz Love"
//  #define WIFI_PASS "bucatar56"
// #define WIFI_SSID "iPhone Station"
// #define WIFI_PASS "blocuri13"
#define WIFI_SSID "Oratoriu_2.4"
#define WIFI_PASS "paulgabriel"

/**
 * @brief Macro to enable or disable the backend testing mode for the web server.
 * @param false
 * @param true
 *
 * @note The value by default is @b false
 *
 * @warning The value of this macro will determine the behavior of the web server. It can either display the real-time sensor data or the testing data.
 */
#define BACKEND_TESTING false
/**
 * @brief Stores the speed value read from the OBD scanner.
 * @note The speed value is in km/h and will be modified by SPEED state.
 */
int speedVariable;

#if (BACKEND_TESTING == true)
int rpmVariable;
#else
/**
 * @brief Stores the rpm value read from the OBD scanner.
 * @note The rpm value will be modified by ENG_RPM state.
 */
uint32_t rpmVariable = 0;
#endif
/**
 * @brief Stores the oil temperature value read from the OBD scanner.
 * @note The oil temperature value will be modified by OIL_TEMP state.
 */
int oilTempVariable;
/**
 * @brief Stores the instant fuel consumption value read from the OBD scanner.
 * @note The instant fuel consumption is measured in L/h and the value will be modified by FUEL_RATE state.
 */
float fuelConsumptionVariable;

/**
 * @brief Typedef for the OBD PID states.
 * @param SPEED
 * @param ENG_RPM
 * @param OIL_TEMP
 * @param FUEL_RATE
 *
 * @note The OBD PID states are used in the state machine.
 */
typedef enum
{
    SPEED,
    ENG_RPM,
    OIL_TEMP,
    FUEL_RATE
} obd_pid_states;
/**
 * @brief Variable which holds the current state of the OBD PID reader.
 * @param SPEED
 * @param ENG_RPM
 * @param OIL_TEMP
 * @param FUEL_RATE
 *
 * @note The OBD PID states are used in the state machine.
 */
obd_pid_states obd_state = SPEED;
/**
 * @brief Vector to hold the historical speed values.
 */
std::vector<float> speedHistory(HISTORY_SIZE, 0);
/**
 * @brief Vector to hold the historical rpm values.
 */
std::vector<int> rpmHistory(HISTORY_SIZE, 0);
/**
 * @brief Index used in vector to keep track of the history.
 * @note This index will be modified when the history is updated.
 */
int historyIndex = 0;
/*-------------------------------------FUNCTIONS-------------------------------------*/
/**
 * @brief Update the history buffers with the latest sensor data.
 * @param void
 * @return void
 */
void updateHistory()
{
    speedHistory.push_back(speedVariable);
    rpmHistory.push_back(rpmVariable);

    if (speedHistory.size() > HISTORY_SIZE)
    {
        speedHistory.erase(speedHistory.begin());
    }
    if (rpmHistory.size() > HISTORY_SIZE)
    {
        rpmHistory.erase(rpmHistory.begin());
    }
}