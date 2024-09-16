# Weather-based Disaster Risk Assessment

A C application that fetches real-time weather data from the [Weatherstack API](https://weatherstack.com/) and assesses disaster risks based on weather conditions. It uses **WinINet** for HTTP requests and parses the JSON response to provide insights.

## Features
- Real-time weather data retrieval.
- Disaster risk assessment (e.g., thunderstorms, heavy rain).
- URL encoding for city names.

## Requirements
- Windows OS with a C compiler (MinGW or Visual Studio).
- Weatherstack API Key.

## Usage
1. Clone the repository:
    ```bash
    git clone https://github.com/your-username/weather-risk-assessment.git
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
Search any Location: London
Country: UK
Timezone: Europe/London
Weather Code: 302
Weather Description: Moderate rain
Wind Speed: 15 km/h
Precipitation: 0.5 mm
