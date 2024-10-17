#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

#define API_KEY "your_API_key_here"
#define MAX_CITY_NAME 100
#define MAX_URL_LENGTH 256
#define MAX_RESPONSE_SIZE 16384

// Function to make an HTTP request
char* make_request(const char* url) {
    HINTERNET hInternet = InternetOpen("WeatherApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        printf("InternetOpen failed\n");
        return NULL;
    }

    HINTERNET hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        printf("InternetOpenUrl failed\n");
        InternetCloseHandle(hInternet);
        return NULL;
    }

    char* buffer = (char*)malloc(MAX_RESPONSE_SIZE);
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return NULL;
    }

    DWORD bytesRead;
    if (!InternetReadFile(hConnect, buffer, MAX_RESPONSE_SIZE - 1, &bytesRead)) {
        printf("InternetReadFile failed\n");
        free(buffer);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return NULL;
    }

    buffer[bytesRead] = 0;

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return buffer;
}

// Function to extract simple key-value data from response
char* extract_value(const char* response, const char* key) {
    char* start = strstr(response, key);
    if (start == NULL) return NULL;
    start = strchr(start, ':');
    if (start == NULL) return NULL;
    start++; // Move past the colon
    while (*start == ' ' || *start == '"') start++; // Skip spaces and opening quote
    char* end = strchr(start, '"');
    if (end == NULL) return NULL;
    int length = end - start;
    char* value = (char*)malloc(length + 1);
    if (value == NULL) return NULL;
    strncpy(value, start, length);
    value[length] = '\0';
    return value;
}

// Function to extract array-like data (e.g., weather descriptions) and remove square brackets
char* extract_array_value(const char* response, const char* key) {
    char* start = strstr(response, key);
    if (start == NULL) return NULL;
    start = strchr(start, '['); // Find the opening bracket
    if (start == NULL) return NULL;
    start++; // Move past the bracket
    while (*start == ' ' || *start == '"') start++; // Skip spaces and quotes
    char* end = strchr(start, '"'); // Find the closing quote
    if (end == NULL) return NULL;
    int length = end - start;
    char* value = (char*)malloc(length + 1);
    if (value == NULL) return NULL;
    strncpy(value, start, length);
    value[length] = '\0';
    return value;
}

void remove_backslashes(char* str) {
    int length = strlen(str);
    int j = 0;
    
    for (int i = 0; i < length; i++) {
        if (str[i] != '\\') {
            str[j++] = str[i];  // Copy the character if it's not a backslash
        }
    }
    
    str[j] = '\0';  // Null-terminate the cleaned string
}


// Function to assess disaster risk based on weather conditions
void check_disaster_risk(const char *weather_code, const char *wind_speed, const char *precipitation) {
    int code = atoi(weather_code);
    float wind = atof(wind_speed);
    float precip = atof(precipitation);

    printf("Disaster risk assessment:\n\n");

    if (code >= 200 && code < 300) {
        printf("- Thunderstorm alert: Seek shelter immediately. Stay away from windows and avoid using electrical appliances. Keep yourself informed of local weather updates.\n\n");
    }
    if (code >= 500 && code < 600) {
        if (precip > 50) {
            printf("- Heavy rainfall: There is a significant risk of flooding in low-lying areas. Be prepared for possible evacuation orders, and ensure that you have a safety kit ready.\n\n");
        } else {
            printf("- Rainfall: Continuous rain may lead to local flooding. Stay alert and monitor weather and emergency alerts for potential escalation.\n\n");
        }
    }
    if (code >= 600 && code < 700) {
        printf("- Snowfall: Roads may become icy and dangerous. It's advised to avoid unnecessary travel. Ensure your heating systems are working and stock up on essentials.\n\n");
    }
    if (wind > 20) {
        printf("- High winds: Strong gusts may damage property. Secure any outdoor furniture or loose objects. Avoid outdoor activities, and stay indoors until the winds subside.\n\n");
    }
    if (code == 781) {
        printf("- Tornado warning: Seek underground shelter immediately, such as a basement or an interior room without windows. If outside, lie flat in a low-lying area and protect your head.\n\n");
    }
    if (code >= 900) {
        printf("- Extreme weather conditions: Expect severe disruptions to daily activities. Follow all instructions from local authorities and evacuate if advised to do so. Your safety is paramount.\n\n");
    } else if (code < 200 && wind < 20) {
        printf("- Weather is calm: Enjoy your day, but remain aware of any sudden changes in weather conditions. Keep an eye on the forecast.\n\n");
    }
}

// Main function
int main() {
    char city_name[MAX_CITY_NAME];
    char url[MAX_URL_LENGTH];

    printf("Search any Location: ");
    fgets(city_name, sizeof(city_name), stdin);
    city_name[strcspn(city_name, "\n")] = 0;  // Remove newline

    // URL encode city name
    char encoded_city_name[MAX_CITY_NAME * 3];
    char *p = encoded_city_name;
    for (const char *c = city_name; *c; c++) {
        if (*c == ' ') {
            *p++ = '%';
            *p++ = '2';
            *p++ = '0';
        } else if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c >= '0' && *c <= '9')) {
            *p++ = *c;
        } else {
            *p++ = '%';
            sprintf(p, "%02X", (unsigned char)*c);
            p += 2;
        }
    }
    *p = '\0';

    snprintf(url, sizeof(url), "http://api.weatherstack.com/current?access_key=%s&query=%s", API_KEY, encoded_city_name);
    char* response = make_request(url);
    if (response) {
        char *weather_code = extract_value(response, "weather_code");
        char *wind_speed = extract_value(response, "wind_speed");
        char *precip = extract_value(response, "precip");
        char *description = extract_array_value(response, "weather_descriptions");
        char *country = extract_value(response, "country");
        char *timezone_id = extract_value(response, "timezone_id");

        if (weather_code && wind_speed && precip && description && country && timezone_id) {
            printf("=============================================================Weather in %s==================================================================\n", city_name);

            printf("Country: %s\n\n", country);
            remove_backslashes(timezone_id);
            printf("Timezone ID: %s\n\n", timezone_id);
            printf("Weather Code: %s\n\n", weather_code);
            printf("Weather Description: %s\n\n", description);
            printf("Wind Speed: %s km/h\n\n", wind_speed);
            printf("Precipitation: %s mm\n\n", precip);

            check_disaster_risk(weather_code, wind_speed, precip);

            // Free allocated memory
            free(weather_code);
            free(wind_speed);
            free(precip);
            free(description);
            free(country);
            free(timezone_id);
        } else {
            printf("Failed to parse response.\n");
        }

        free(response);
    } else {
        printf("Failed to get weather data.\n");
    }

    return 0;
}
