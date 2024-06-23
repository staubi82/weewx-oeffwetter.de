#encoding UTF-8
<div class="entry-content blog-outer" itemprop="text">
    <br>
  <img style="display: block; margin-left: auto; margin-right: auto;" src="/images/oeffingen.png" alt="" />
    <br><br>
<body>
    <h1 class="code-line" data-line-start="0" data-line-end="1"><a id="Weewx_50__GW1100___CloudPanel__Tailscale__Netcup_bzw_Hetzner_0"></a>Weewx 5.0 + GW1100 + CloudPanel + Tailscale + Netcup bzw Hetzner</h1>
    <p class="has-line-data" data-line-start="2" data-line-end="3">Wir installieren auf einem Debian 11 CloudPanel f&uuml;r die einfache Einrichtung von SSL-Zertifikaten und zur Absicherung des Servers. Zus&auml;tzlich nutzen wir Tailscale f&uuml;r eine sichere und unkomplizierte VPN-Verbindung. Um sicherzustellen, dass WeeWX alle ben&ouml;tigten Daten erh&auml;lt, verwende ich das Gateway GW1000 von Ecowitt. Die Einbindung neuer Sensoren ist hier besonders einfach und ihre &Uuml;bertragung an WeeWX unkompliziert. Durch Mosquitto wird ein MQTT-Broker bereitgestellt, der die Abfrage von Echtzeit-Wetterdaten erm&ouml;glicht.</p>
    <p class="has-line-data" data-line-start="4" data-line-end="5">In den folgenden Schritten beschreibe ich ausf&uuml;hrlich, wie Sie Ihre eigene Wetterseite erstellen k&ouml;nnen. Grundlegende Erfahrungen im Umgang mit Linux werden vorausgesetzt. Bei Unsicherheiten oder dem Bedarf an zus&auml;tzlichen Erkl&auml;rungen stehe ich Ihnen gerne zur Verf&uuml;gung.</p>
    <p class="has-line-data" data-line-start="6" data-line-end="7">Ich hoste WeeWX lieber auf einem Webhoster als zuhause. Sollte die Seite einmal angegriffen oder anderweitig beeintr&auml;chtigt werden, erfolgt dies ausschlie&szlig;lich auf dem Webhoster und nicht in meinem Heimnetzwerk. Daher ist es erforderlich, mit Tailscale ein VPN zwischen dem Server und dem Heimnetzwerk einzurichten.</p>
    <p class="has-line-data" data-line-start="8" data-line-end="9">Es gibt auch M&ouml;glichkeiten, die Daten von der Wetterstation mit dem Ecowitt-Protokoll an die Wetterstation zu senden. Diese Option wird hier jedoch nicht weiter behandelt.</p>
    <h2 class="code-line" data-line-start="10" data-line-end="11"><a id="Disclaimer_Danke_Deutschland_10"></a>Disclaimer (Danke Deutschland):</h2>
    <p class="has-line-data" data-line-start="12" data-line-end="13">Diese Anleitung dient lediglich als pers&ouml;nliche Darstellung meines eigenen Ansatzes zur Einrichtung einer Wetterseite. Es gibt viele verschiedene Wege und Methoden, dies zu tun, und meine Vorgehensweise repr&auml;sentiert lediglich eine von vielen M&ouml;glichkeiten. Jegliche Umsetzung oder Anwendung der hier beschriebenen Schritte erfolgt auf eigene Verantwortung. Ich &uuml;bernehme keine Haftung f&uuml;r etwaige Sch&auml;den oder Unannehmlichkeiten, die durch die Verwendung dieser Anleitung entstehen k&ouml;nnten. Benutzer werden ermutigt, ihre eigenen Anforderungen und Umst&auml;nde zu ber&uuml;cksichtigen und gegebenenfalls professionelle Beratung einzuholen.</p>
    <h3 class="code-line" data-line-start="14" data-line-end="15"><a id="Voraussetzungen_14"></a>Voraussetzungen:</h3>
    <ul>
    <li class="has-line-data" data-line-start="16" data-line-end="21">
    <h4 class="code-line" data-line-start="16" data-line-end="17"><a id="Hardware_16"></a>Hardware</h4>
    <ul>
    <li class="has-line-data" data-line-start="17" data-line-end="18">Wetterstation</li>
    <li class="has-line-data" data-line-start="18" data-line-end="19">GW1000 (IP sollte bekannt sein)</li>
    <li class="has-line-data" data-line-start="19" data-line-end="20">Homeserver (zb. Raspberry pi) (wird f&uuml;r Tailscale gebraucht)</li>
    <li class="has-line-data" data-line-start="20" data-line-end="21">Internet</li>
    </ul>
    </li>
    <li class="has-line-data" data-line-start="21" data-line-end="26">
    <h4 class="code-line" data-line-start="21" data-line-end="22"><a id="Server_21"></a>Server</h4>
    <ul>
    <li class="has-line-data" data-line-start="22" data-line-end="23">Server mit Debian 11 (12 wird aktuell nicht unterst&uuml;tzt)</li>
    <li class="has-line-data" data-line-start="23" data-line-end="24">Domain inklusive DNS-Eintr&auml;ge auf dem Server</li>
    <li class="has-line-data" data-line-start="24" data-line-end="26">SSH-Zugriff mit Root-Rechten</li>
    </ul>
    </li>
    </ul>
    <h2 class="code-line" data-line-start="26" data-line-end="27"><a id="Installation_26"></a>Installation</h2>
    <p class="has-line-data" data-line-start="27" data-line-end="28">Wir starten mit CloudPanel, da alle weiteren Schritte darauf aufbauen:</p>
    <pre><code class="has-line-data" data-line-start="29" data-line-end="31">apt update &amp;&amp; apt -y upgrade &amp;&amp; apt -y install curl wget sudo</code></pre>
    <p class="has-line-data" data-line-start="31" data-line-end="32">Nachdem alles aktualisiert und installiert wurde, f&uuml;hren wir den Befehl aus, um CloudPanel zu installieren:</p>
    <h5 class="code-line" data-line-start="33" data-line-end="34"><a id="fr_alle_Hoster_auer_Hetzner_33"></a>f&uuml;r alle Hoster au&szlig;er Hetzner</h5>
    <pre><code class="has-line-data" data-line-start="35" data-line-end="39">curl <span class="hljs-operator">-s</span>S https://installer.cloudpanel.io/ce/v2/install.sh -o install.sh; \
        <span class="hljs-built_in">echo</span> <span class="hljs-string">"85762db0edc00ce19a2cd5496d1627903e6198ad850bbbdefb2ceaa46bd20cbd install.sh"</span> | \
        sha256sum -c &amp;&amp; sudo bash install.sh</code></pre>
    <h5 class="code-line" data-line-start="39" data-line-end="40"><a id="fr_Hetzner_39"></a>f&uuml;r Hetzner</h5>
    <pre><code class="has-line-data" data-line-start="41" data-line-end="45">curl <span class="hljs-operator">-s</span>S https://installer.cloudpanel.io/ce/v2/install.sh -o install.sh; \
        <span class="hljs-built_in">echo</span> <span class="hljs-string">"85762db0edc00ce19a2cd5496d1627903e6198ad850bbbdefb2ceaa46bd20cbd install.sh"</span> | \
        sha256sum -c &amp;&amp; sudo CLOUD=hetzner bash install.sh</code></pre>
    <p class="has-line-data" data-line-start="46" data-line-end="47">Das kann ein wenig dauern. Wenn die Installation durchgelaufen ist, rufen wir die Internetseite auf, die im Terminal angezeigt wird.</p>
    <h4 class="code-line" data-line-start="47" data-line-end="48"><a id="CloudPanal_einrichtung_47"></a>CloudPanal einrichtung</h4>
    <ul>
    <li class="has-line-data" data-line-start="48" data-line-end="49">Benutzer erstellen</li>
    <li class="has-line-data" data-line-start="49" data-line-end="50">Im Admin-Bereich unter Einstellungen bitte die Domain mit Subdomain (z.B., <code>cp.webseite.de</code>) eintragen.</li>
    <li class="has-line-data" data-line-start="50" data-line-end="51">Anschlie&szlig;end rufen wir <code>cp.webseite.de</code> auf und melden uns an.</li>
    <li class="has-line-data" data-line-start="51" data-line-end="52">Dann erstellen wir eine neue Website mit <em>&ldquo;Webseite Hinzuf&uuml;gen&rdquo;</em>.</li>
    <li class="has-line-data" data-line-start="52" data-line-end="53">Jetzt unbedingt <em>PHP-Site</em> ausw&auml;hlen.</li>
    <li class="has-line-data" data-line-start="53" data-line-end="54">Als Domainname die Domain verwenden, unter der die Wetterseite erreichbar sein soll.</li>
    <li class="has-line-data" data-line-start="54" data-line-end="55">Bei Benutzer muss <em>weewx</em> ausgew&auml;hlt werden, um sp&auml;tere Rechtekonflikte zu vermeiden.</li>
    <li class="has-line-data" data-line-start="55" data-line-end="56">Nachdem die Seite erstellt wurde, klicken wir auf <em>Verwalten</em> und dann im Reiter <em>SSL/TLS</em>.</li>
    <li class="has-line-data" data-line-start="56" data-line-end="57">Dort auf den Button <em>Aktion</em> klicken, um ein SSL-Zertifikat zu erstellen.</li>
    <li class="has-line-data" data-line-start="57" data-line-end="59">Das war es mit der Einrichtung.</li>
    </ul>
    <h4 class="code-line" data-line-start="59" data-line-end="60"><a id="Tailscale_Installation_und_Einrichtung_59"></a>Tailscale Installation und Einrichtung</h4>
    <p class="has-line-data" data-line-start="61" data-line-end="64">Zun&auml;chst sollte Tailscale auf dem Home-Server installiert und eingerichtet sein. Hierf&uuml;r gibt es zahlreiche Anleitungen im Internet.<br /> Nun setzen wir auf unserem Wetter-Server fort:<br /> Wir f&uuml;hren den folgenden Befehl aus, um die Installation zu beginnen:</p>
    <pre><code class="has-line-data" data-line-start="66" data-line-end="68">curl -fsSL https://tailscale.com/install.sh | sh</code></pre>
    <p class="has-line-data" data-line-start="68" data-line-end="69">Anschlie&szlig;end sollte der Befehl</p>
    <pre><code class="has-line-data" data-line-start="70" data-line-end="72">tailscale up</code></pre>
    <p class="has-line-data" data-line-start="72" data-line-end="73">genutzt werden, um die Login-URL zu erhalten. Dann einfach in den Tailscale-Account einloggen und das Ger&auml;t authentifizieren. Um den Service zu stoppen, verwenden Sie:</p>
    <pre><code class="has-line-data" data-line-start="74" data-line-end="76">tailscale down</code></pre>
    <p class="has-line-data" data-line-start="76" data-line-end="77">Danach kann der Service erneut gestartet werden, um auf den GW1000 zuzugreifen:</p>
    <pre><code class="has-line-data" data-line-start="78" data-line-end="80">tailscale up --accept-routes</code></pre>
    <p class="has-line-data" data-line-start="80" data-line-end="81">Sie k&ouml;nnen die Verbindung testen, indem Sie einen Ping auf den GW1000 versuchen. Verwenden Sie dazu den Befehl:</p>
    <pre><code class="has-line-data" data-line-start="83" data-line-end="85">ping GW1000-IP-Adresse<br /></code></pre>
    <p class="has-line-data" data-line-start="85" data-line-end="86">Ersetzen Sie &ldquo;GW1000-IP-Adresse&rdquo; durch die tats&auml;chliche IP-Adresse des GW1000-Ger&auml;ts. Wenn der Ping erfolgreich ist, besteht eine erfolgreiche Verbindung zu Ihrem GW1000.</p>
    <h4 class="code-line" data-line-start="87" data-line-end="88"><a id="Weewx_Installation_87"></a>Weewx Installation</h4>
    <p class="has-line-data" data-line-start="88" data-line-end="89">Als Erstes sollten wir &uuml;berpr&uuml;fen, ob die Zeitzone richtig eingestellt ist. Verwenden Sie dazu den folgenden Befehl:</p>
    <pre><code class="has-line-data" data-line-start="91" data-line-end="93">date</code></pre>
    <p class="has-line-data" data-line-start="93" data-line-end="94">Wenn die Zeit korrekt eingestellt ist, k&ouml;nnen Sie den n&auml;chsten Schritt &uuml;berspringenn.</p>
    <p class="has-line-data" data-line-start="95" data-line-end="96">Wenn die Zeit nicht korrekt eingestellt ist, liegt das h&auml;ufig an einer falschen Zeitzone. Sie k&ouml;nnen die Zeitzone mit dem Befehl <code>dpkg-reconfigure tzdata</code> &auml;ndern. Verwenden Sie dazu:</p>
    <pre><code class="has-line-data" data-line-start="98" data-line-end="100">dpkg-reconfigure tzdata</code></pre>
    <p class="has-line-data" data-line-start="100" data-line-end="101">Folgen Sie den Anweisungen, um die gew&uuml;nschte Zeitzone auszuw&auml;hlen. Nach der Anpassung k&ouml;nnen Sie die Zeit erneut &uuml;berpr&uuml;fen, um sicherzustellen, dass sie korrekt eingestellt ist.</p>
    <p class="has-line-data" data-line-start="102" data-line-end="103">Nun sollten wir zuerst einige Abh&auml;ngigkeiten installieren. Verwenden Sie dazu den Befehl:</p>
    <pre><code class="has-line-data" data-line-start="105" data-line-end="107">sudo apt install python3-ephem python3-pcapy unzip gnupg gpg python3-paho-mqtt</code></pre>
    <p class="has-line-data" data-line-start="107" data-line-end="108">Nun f&uuml;gen wir die Repositories f&uuml;r WeeWX hinzu. Verwenden Sie die folgenden Befehle:</p>
    <pre><code class="has-line-data" data-line-start="109" data-line-end="111">wget -qO - https://weewx.com/keys.html | sudo gpg --dearmor --output /etc/apt/trusted.gpg.d/weewx.gpg</code></pre>
    <pre><code class="has-line-data" data-line-start="112" data-line-end="114"><span class="hljs-built_in">echo</span> <span class="hljs-string">"deb [arch=all] https://weewx.com/apt/python3 buster main"</span> | sudo tee /etc/apt/sources.list.d/weewx.list</code></pre>
    <p class="has-line-data" data-line-start="115" data-line-end="116">Jetzt installieren wir endlich WeeWX. Verwenden Sie den folgenden Befehl:</p>
    <pre><code class="has-line-data" data-line-start="118" data-line-end="120">apt update &amp;&amp; apt install weewx -y</code></pre>
    <p class="has-line-data" data-line-start="121" data-line-end="122">Bei Fehlermeldung:</p>
    <pre><code class="has-line-data" data-line-start="123" data-line-end="125">wget -qO - https://weewx.com/keys-old.html | sudo gpg --dearmor --output /etc/apt/trusted.gpg.d/weewx.gpg</code></pre>
    <p class="has-line-data" data-line-start="125" data-line-end="126">und dann nochmal versuchen mit &ldquo;install weewx&rdquo;</p>
    <p class="has-line-data" data-line-start="127" data-line-end="129">Jetzt wird WeeWX nach einigen Daten fragen. Sie k&ouml;nnen diese entweder eingeben oder &uuml;berspringen, da Sie sie sp&auml;ter noch konfigurieren k&ouml;nnen.<br /> Als N&auml;chstes installieren wir den Treiber f&uuml;r das GW1000. Verwenden Sie den folgenden Befehl:</p>
    <pre><code class="has-line-data" data-line-start="130" data-line-end="132">wget https://github.com/gjr80/weewx-gw1000/releases/download/v0.<span class="hljs-number">6.0</span>/gw1000.zip</code></pre>
    <pre><code class="has-line-data" data-line-start="133" data-line-end="135">weectl extension install gw1000.zip</code></pre>
    <pre><code class="has-line-data" data-line-start="136" data-line-end="138">weectl station reconfigure</code></pre>
    <p class="has-line-data" data-line-start="138" data-line-end="139">Jetzt sollten Sie die Daten der Wetterstation eingeben. Es ist wichtig, die korrekten Koordinaten zu verwenden. W&auml;hlen Sie den Treiber &ldquo;GW1000&rdquo; und tragen Sie bei &ldquo;GW1000 URL&rdquo; die IP-Adresse des GW1000 ein. Der Port sollte auf 4500 eingestellt sein.</p>
    <p class="has-line-data" data-line-start="140" data-line-end="141">Wir installieren direkt das Belchertown Skin. Verwenden Sie dazu den folgenden Befehl:</p>
    <pre><code class="has-line-data" data-line-start="142" data-line-end="144">wget https://github.com/poblabs/weewx-belchertown/releases/download/weewx-belchertown-<span class="hljs-number">1.3</span>.<span class="hljs-number">1</span>/weewx-belchertown-release.<span class="hljs-number">1.3</span>.<span class="hljs-number">1</span>.tar.gz</code></pre>
    <pre><code class="has-line-data" data-line-start="145" data-line-end="147">weectl extension install weewx-belchertown-release.<span class="hljs-number">1.3</span>.<span class="hljs-number">1</span>.tar.gz</code></pre>
    <p class="has-line-data" data-line-start="148" data-line-end="149">Jetzt installieren wir noch das MQTT-Paket.</p>
    <pre><code class="has-line-data" data-line-start="150" data-line-end="152">wget -O weewx-mqtt.zip https://github.com/matthewwall/weewx-mqtt/archive/master.zip</code></pre>
    <pre><code class="has-line-data" data-line-start="153" data-line-end="155">weectl extension install weewx-mqtt.zip</code></pre>
    <p class="has-line-data" data-line-start="156" data-line-end="158">Sie k&ouml;nnen nat&uuml;rlich auch weitere Erweiterungen auf die gleiche Art installieren.<br /> F&uuml;rs Erste sind wir fertig. MQTT kommt sp&auml;ter.</p>
    <h4 class="code-line" data-line-start="159" data-line-end="160"><a id="Jetzt_sind_wir_mit_der_Installation_fertig_nun_geht_es_ans_Konfigurieren_159"></a>Jetzt sind wir mit der Installation fertig, nun geht es ans Konfigurieren.</h4>
    <p class="has-line-data" data-line-start="162" data-line-end="163">&Ouml;ffnen Sie die Datei <code>/etc/weewx/weewx.conf</code> mit einem Editor Ihrer Wahl. Ich pers&ouml;nlich nutze Nano, aber Sie k&ouml;nnen gerne Ihren bevorzugten Editor verwenden. Verwenden Sie den folgenden Befehl:</p>
    <pre><code class="has-line-data" data-line-start="165" data-line-end="167">nano /etc/weewx/weewx.conf</code></pre>
    <h5 class="code-line" data-line-start="167" data-line-end="168"><a id="Achtung_Wichtig_167"></a>Achtung Wichtig!</h5>
    <p class="has-line-data" data-line-start="168" data-line-end="169">Nach Belieben k&ouml;nnen Sie die WeeWX-Konfigurationsdatei nach Ihren W&uuml;nschen einrichten. Allerdings sollten Sie unbedingt die Einstellung &ldquo;HTML_ROOT = /home/weewx/htdocs/wetter-url&rdquo; anpassen. Dies muss sowohl unter [StdReport] als auch beim [[Belchertown]]-Skin ge&auml;ndert werden.</p>
    <p class="has-line-data" data-line-start="170" data-line-end="171">Des Weiteren sollte beim [[SeasonsReport]] die Einstellung von &ldquo;enable = true&rdquo; auf &ldquo;enable = false&rdquo; ge&auml;ndert werden.</p>
    <p class="has-line-data" data-line-start="172" data-line-end="173">Ich pers&ouml;nlich setze bei [StdArchive] die Einstellung &ldquo;archive_interval&rdquo; von 300 auf 60 Sekunden herunter. Dies erm&ouml;glicht k&uuml;rzere Intervalle f&uuml;r Archivdaten.</p>
    <p class="has-line-data" data-line-start="174" data-line-end="175">Nachdem alle Konfigurationen vorgenommen wurden, k&ouml;nnen Sie WeeWX neu starten. Die Wetterdaten sollten nun auf Ihrer Webseite angezeigt werden.</p>
    <pre><code class="has-line-data" data-line-start="176" data-line-end="178">systemctl restart weewx.service</code></pre>
    <p class="has-line-data" data-line-start="179" data-line-end="180">Es kann bis zu 5 Minuten dauern, bis die ersten Daten auf Ihrer Wetterseite erscheinen, abh&auml;ngig von der bei &ldquo;archive_interval&rdquo; gew&auml;hlten Zeitspanne. Haben Sie bitte Geduld.</p>
    <p class="has-line-data" data-line-start="181" data-line-end="182">Alternativ k&ouml;nnen Sie mit dem folgenden Befehl das Geschehen live verfolgen:</p>
    <pre><code class="has-line-data" data-line-start="184" data-line-end="186">journalctl <span class="hljs-operator">-f</span></code></pre>
    <p class="has-line-data" data-line-start="186" data-line-end="187">Das Belchertown Skin k&ouml;nnen Sie mit der Datei <code>/etc/weewx/skins/Belchertown/skin.conf</code> konfigurieren.</p>
    <pre><code class="has-line-data" data-line-start="188" data-line-end="190">nano /etc/weewx/skins/Belchertown/skin.conf</code></pre>
    <p class="has-line-data" data-line-start="190" data-line-end="191">Mit den abgeschlossenen Konfigurationen und Installationen sollte Ihr Wetterdienst nun reibungslos funktionieren. Falls Sie weitere Unterst&uuml;tzung, Anpassungen oder Informationen ben&ouml;tigen, stehe ich Ihnen jederzeit gerne zur Verf&uuml;gung.</p>
    <p class="has-line-data" data-line-start="192" data-line-end="193">Zus&auml;tzlich besteht die M&ouml;glichkeit, sich mit dem optionalen Thema MQTT zu befassen. F&uuml;r eine ausf&uuml;hrliche Anleitung dazu werde ich ein separates Tutorial erstellen.</p>
</div>