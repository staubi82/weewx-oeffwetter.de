#!/bin/bash

# System aktualisieren und notwendige Pakete installieren
apt update && apt -y upgrade && apt -y install wget debconf-utils locales git
apt install python3-ephem python3-pcapy unzip gnupg gpg python3-paho-mqtt nginx -y

# Deutsche Sprachunterstützung installieren
echo "de_DE.UTF-8 UTF-8" > /etc/locale.gen
locale-gen
update-locale LANG=de_DE.UTF-8

# Überprüfen und setzen der Zeitzone auf Europe/Berlin
CURRENT_TZ=$(timedatectl show --property=Timezone --value)
if [ "$CURRENT_TZ" != "Europe/Berlin" ]; then
    timedatectl set-timezone Europe/Berlin
fi

# Weewx GPG-Schlüssel hinzufügen und Repository einrichten
wget -qO - https://weewx.com/keys.html | gpg --dearmor --output /etc/apt/trusted.gpg.d/weewx.gpg
echo "deb [arch=all] https://weewx.com/apt/python3 buster main" | tee /etc/apt/sources.list.d/weewx.list

# Weewx installieren mit Standardwerten
DEBIAN_FRONTEND=noninteractive apt update && DEBIAN_FRONTEND=noninteractive apt install weewx -y

# GW1000 Erweiterung installieren
wget https://github.com/gjr80/weewx-gw1000/releases/download/v0.6.0/gw1000.zip
weectl extension install gw1000.zip -y

# Belchertown Erweiterung installieren
wget https://github.com/poblabs/weewx-belchertown/releases/download/weewx-belchertown-1.3.1/weewx-belchertown-release.1.3.1.tar.gz
weectl extension install weewx-belchertown-release.1.3.1.tar.gz -y

# MQTT Erweiterung installieren
wget -O weewx-mqtt.zip https://github.com/matthewwall/weewx-mqtt/archive/master.zip
weectl extension install weewx-mqtt.zip -y

# Temporäres Verzeichnis erstellen
mkdir /tmp/weewx-config

# Repository klonen (über HTTPS)
git clone https://github.com/staubi82/weewx.git /tmp/weewx-config

# Überprüfen, ob das Klonen erfolgreich war
if [ $? -ne 0 ]; then
    echo "Fehler beim Klonen des Repositories"
    exit 1
fi

# Dateien kopieren
cp -r /tmp/weewx-config/* /etc/weewx/

# Überprüfen, ob das Kopieren erfolgreich war
if [ $? -ne 0 ]; then
    echo "Fehler beim Kopieren der Dateien"
    exit 1
fi

# Temporäres Verzeichnis löschen
rm -rf /tmp/weewx-config

# Berechtigungen für /var/www/html/ setzen
chown weewx:weewx /var/www/html/

echo "Konfigurationsdateien erfolgreich kopiert. Bitte bearbeiten Sie /etc/weewx/weewx.conf und fügen Sie die PWSweather-Daten hinzu."
echo "Anschließend können Sie den Weewx-Dienst mit 'systemctl restart weewx' neu starten."

# Hinweis für PWSweather-Konfiguration in weewx.conf einfügen