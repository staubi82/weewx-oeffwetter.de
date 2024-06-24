

# Weewx Installationsskript

Dieses Skript automatisiert die Installation und Konfiguration von Weewx auf einem Debian-basierten System. Es führt die folgenden Schritte aus:

## Installation

Führen Sie den folgenden Befehl als Root-Benutzer aus, um das Skript direkt von GitHub herunterzuladen und auszuführen:

```bash
wget -qO - https://raw.githubusercontent.com/staubi82/weewx/main/install.sh | bash
```

## Funktionen des Skripts

1. **Systemaktualisierung und Paketinstallation**
    - Aktualisiert das System (`apt update && apt -y upgrade`).
    - Installiert notwendige Pakete (`wget`, `debconf-utils`, `locales`, `git`, `python3-ephem`, `python3-pcapy`, `unzip`, `gnupg`, `gpg`, `python3-paho-mqtt`, `nginx`).

2. **Installation der deutschen Sprachunterstützung**
    - Fügt `de_DE.UTF-8` zur `locale.gen` hinzu und generiert die Locale.
    - Setzt die Standard-Locale auf `de_DE.UTF-8`.

3. **Zeitzone setzen**
    - Überprüft und setzt die Zeitzone auf `Europe/Berlin`, falls diese nicht bereits eingestellt ist.

4. **Weewx-Installation**
    - Fügt den Weewx GPG-Schlüssel hinzu.
    - Konfiguriert das Weewx-Repository.
    - Installiert Weewx mit Standardwerten (`DEBIAN_FRONTEND=noninteractive`).

5. **Installation von Weewx-Erweiterungen**
    - Installiert die GW1000 Erweiterung.
    - Installiert die Belchertown Erweiterung.
    - Installiert die MQTT Erweiterung.

6. **Klonen und Kopieren der Konfigurationsdateien**
    - Klont das GitHub-Repository `https://github.com/staubi82/weewx.git` in ein temporäres Verzeichnis.
    - Kopiert die Konfigurationsdateien nach `/etc/weewx/`.

7. **Berechtigungen setzen**
    - Setzt die Berechtigungen für `/var/www/html/` auf `weewx:weewx`.

8. **PWSweather-Konfiguration**
    - Fragt den Benutzer, ob Daten an PWSweather.com gesendet werden sollen (`true/false`).
    - Fragt nach dem Stationsnamen und dem Passwort für PWSweather.com.
    - Trägt die Benutzereingaben in die `weewx.conf` ein.

9. **Neustart und Statusüberprüfung des Weewx-Dienstes**
    - Startet den Weewx-Dienst neu und zeigt den Status an.

## Hinweise

- Das Skript wird als Root-Benutzer ausgeführt und erfordert keine Verwendung von `sudo`.
- Überprüfen und validieren Sie die Inhalte des Skripts, bevor Sie es ausführen, um sicherzustellen, dass es keine unerwünschten Auswirkungen auf Ihr System hat.
- Stellen Sie sicher, dass die URL sicher und vertrauenswürdig ist, da das Skript direkt ausgeführt wird.
```

Dieses Markdown-Dokument kann als `README.md` in deinem GitHub-Repository hinzugefügt werden. Es enthält eine kurze Beschreibung des Skripts und führt die Schritte auf, die das Skript ausführt. Der `wget`-Befehl am Anfang ermöglicht es dem Benutzer, das Skript einfach herunterzuladen und auszuführen.