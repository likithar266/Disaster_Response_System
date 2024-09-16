# Weather-based Disaster Risk Assessment

A C application that fetches real-time weather data using the Windows API (WinINet) and assesses disaster risks based on weather conditions. It processes the data without JSON parsing.

## Features
- Real-time weather data retrieval using WinINet.
- Disaster risk assessment (e.g., thunderstorms, heavy rain).
- URL encoding for city names.

## Requirements
- Windows OS with a C compiler (MinGW or Visual Studio).
- Weatherstack API Key.

## Usage
1. Clone the repository:
    ```bash
    git clone https://github.com/likithar266/Disaster_Response_System.git
    ```
2. Replace `API_KEY` in the code with your Weatherstack API key.
3. Compile the code:
    ```bash
    gcc main.c -o weather -lwininet
    ```
4. Run the application:
    ```bash
    ./weather
    ```

## Example
```bash
Search any Location: India
--------------------------Weather in India---------------------------
Country: India
Timezone ID: Asia/Kolkata
Weather Code: 143
Weather Description: Haze
Wind Speed: 15 km/h
Precipitation: 0.5 mm
Disaster risk assessment:
    -Weather is calm: Enjoy your day, but remain aware of any sudden changes in weather conditions. Keep an eye on the forecast.   
