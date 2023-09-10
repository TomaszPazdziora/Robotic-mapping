import requests
from bs4 import BeautifulSoup

URL = "https://meteogram.pl/slonce/polska/linia/"

def get_angles():
    # pobranie zawartości strony
    response = requests.get(URL)
    html = response.content

    # utworzenie obiektu BeautifulSoup z zawartością strony
    soup = BeautifulSoup(html, "html.parser")

    # znalezienie elementu zawierającego wartość azymutu
    azimuth_span = soup.find("span", {"id": "azimuth"})
    altitude_span = soup.find("span", {"id": "altitude"})

    # pobranie wartości azymutu ze znacznika span
    azimuth_value = azimuth_span.text.split("°")[0]
    altitude_value = altitude_span.text.split("°")[0]

    return (azimuth_value + ' ' + altitude_value)


def get_azimuth():
    response = requests.get(URL)
    html = response.content
    soup = BeautifulSoup(html, "html.parser")
    azimuth_span = soup.find("span", {"id": "azimuth"})
    azimuth_value = azimuth_span.text.split("°")[0]

    return (azimuth_value)


def get_altitude():
    response = requests.get(URL)
    html = response.content
    soup = BeautifulSoup(html, "html.parser")
    altitude_span = soup.find("span", {"id": "altitude"})
    altitude_value = altitude_span.text.split("°")[0]

    return (altitude_value)