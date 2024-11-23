/****************************************************************************/
/*                                                                          */
/* Copyright (C) 2023-2024 Gauthier Brière (gauthier.briere "at" gmail.com) */
/*                                                                          */
/* This file: script.js is part of WeTimer / WeDT                           */
/*                                                                          */
/* WeTimer / WeDT is free software: you can redistribute it and/or modify   */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* WeTimer / WeDT is distributed in the hope that it will be useful, but    */
/* WITHOUT ANY WARRANTY; without even the implied warranty of               */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*                                                                          */
/****************************************************************************/

// pour debug du developpement, adresse IP de la Wemos connectée au wifi
//var netDevURL = 'http://10.11.11.11'; // connected to WeThermic
var netDevURL = 'http://192.168.1.102'; // DomoPassaduy

var attenteOK       = false;

async function index_onload() {

  // Récupère la liste des réseaux disponibles
  get_networks();

}

function XMLHttpRequest_get(requette) {
  // Requette XML HTTP GET
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == 4) {
      if ((xhttp.status == 200) || (xhttp.status == 0)) {
        XMLHttpResult(requette, xhttp.responseXML, xhttp.responseText);
      } else {
        alert("XMLHttpRequest_get(" + requette + ") : Error " + xhttp.status);
      }
    }
  };
  xhttp.open("GET", requette, true);
  xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
  xhttp.send();
}

function XMLHttpResult(requette, xml, text) {
  // Traitement de la réponse XML HTTP GET si existe...
  if (xml != null) {
    if ((requette == "/getnetworks") || (requette == netDevURL + "/getnetworks")) {
      // Rempli la liste des réseaux disponibles
      setNetworkList(xml);
      attenteOK = true;

    } else if (requette == "/wificonnect") {
      result = xml.getElementsByTagName("result")[0].childNodes[0].nodeValue;
      if (result == "OK") {
        alert("Connexion OK.");
      } else {
        alert("Connexion error: \n" + result);
      }
    }
  }
}

async function get_networks() {

  // Efface les données précédentes
  var divNetworkTable = document.getElementById("networkTable");
  divNetworkTable.innerHTML = "";
  var ssidActuel   = document.getElementById("ssidActuel");
  ssidActuel.value = "";
  var ipActuel     = document.getElementById("ipActuel");
  ipActuel.value   = "";

  // envoi la requette et attend la réponse
  attenteOK = false;
  if (location.protocol == 'file:') {
    XMLHttpRequest_get(netDevURL + "/getnetworks");
  } else {
    XMLHttpRequest_get("/getnetworks");
  }
  while (!attenteOK) {
    await sleep(100);
  }
  attenteOK = false;
}

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

function setNetworkList(xml) {
  // Fonction appelée en retour de XMLHttpRequest_get("/getnetworks")
  var htmlNetworkTable = "";
  var tmpSSID      = "";
  var tmpChannel   = 0;
  var tmpRSSI      = "";
  var tmpCrypt     = "";
  var tmpPasswd    = "";
  var tmpImgSignal = "";
  
  var divNetworkTable = document.getElementById("networkTable");
  
  htmlNetworkTable += "  <table class=\"netTables\">\n";
  htmlNetworkTable += "    <thead>\n";
  htmlNetworkTable += "      <tr>\n";
  htmlNetworkTable += "        <th>&nbsp;</th>\n";
  htmlNetworkTable += "        <th>SSID</th>\n";
  htmlNetworkTable += "        <th>Channel</th>\n";
  htmlNetworkTable += "        <th>Security</th>\n";
  htmlNetworkTable += "      </tr>\n";
  htmlNetworkTable += "    </thead>\n";
  htmlNetworkTable += "    <tbody>\n";


  var netCurrent = xml.getElementsByTagName("activeNetwork")
  if (netCurrent.length == 1) {
    var ssidActuel   = document.getElementById("ssidActuel");
    currentSSID      = netCurrent[0].getElementsByTagName("SSID")[0].childNodes[0].nodeValue;
    ssidActuel.value = currentSSID;
    var ipActuel     = document.getElementById("ipActuel");
    currentIP        = netCurrent[0].getElementsByTagName("localip")[0].childNodes[0].nodeValue;
    ipActuel.value   = currentIP;
  }
  
  var netListe = xml.getElementsByTagName("network")
  if (netListe.length > 0) {
    for (var i = 0; i< netListe.length; i++) {
      // Données du réseau
      tmpSSID    = netListe[i].getElementsByTagName("SSID")[0].textContent;
      tmpChannel = netListe[i].getElementsByTagName("channel")[0].textContent;
      tmpRSSI    = netListe[i].getElementsByTagName("RSSI")[0].textContent;
      tmpCrypt   = netListe[i].getElementsByTagName("encryption")[0].textContent;
      // Choix du pictogramme en fonction de la qualité du signal
      // RSSI (dBm) Interprétation
      // -30 dBm    Extraordinaire (êtes vous assis sur la borne? ^^)
      // -67 dBm    Très bon signal
      // -70 dBm    Très acceptable
      // -80 dBm   	as terrible du tout
      // -90 dBm    Inutilisable
      if (tmpRSSI <= -90) {
        tmpImgSignal = "images/signal0.svg";
      } else if ((tmpRSSI > -90) && (tmpRSSI <= -80)) {
        tmpImgSignal = "images/signal1.svg";
      } else if ((tmpRSSI > -80) && (tmpRSSI <= -70)) {
        tmpImgSignal = "images/signal2.svg";
      } else if ((tmpRSSI > -70) && (tmpRSSI <= -67)) {
        tmpImgSignal = "images/signal3.svg";
      } else if (tmpRSSI > -67) {
        tmpImgSignal = "images/signal4.svg";
      }
      htmlNetworkTable += "      <tr class=\"trlink\" onclick=\"wifi_connect('" + tmpSSID + "', '" + tmpChannel + "')\">\n";
      htmlNetworkTable += "        <td class=\"celluleCentree\"><img src=\"" + tmpImgSignal + "\" title=\"RSSI = " + tmpRSSI + "\" /></td>\n";
      htmlNetworkTable += "        <td>" + tmpSSID + "</td>\n";
      htmlNetworkTable += "        <td>" + tmpChannel + "</td>\n";
      htmlNetworkTable += "        <td>" + tmpCrypt + "</td>\n";
      htmlNetworkTable += "      </tr>\n";
    }
  } else {
    htmlNetworkTable   += "      <tr>\n";
      htmlNetworkTable += "        <td colspan=\"4\">No network available</td>";
    htmlNetworkTable   += "      </tr>\n";
  }
  htmlNetworkTable += "    </tbody>\n";
  htmlNetworkTable += "  </table>\n";
  
  divNetworkTable.innerHTML = htmlNetworkTable;

  var attente = document.getElementById("attente0")
  attente.classList.add("noshow");

}

async function wifi_connect(SSID, channel) {

  var pwd = "";

  var ssid_input = document.getElementById("ssid_input");
  var pwd_input  = document.getElementById("pwd_input");
  var divWait    = document.getElementById("attente0");
  var btnConnect = document.getElementById("btnConnect");
  var btnAnnuler = document.getElementById("btnAnnuler");

  // Force le mode input masquée si une connexion précédente l'a modifié
  const bouton = document.getElementById('btnShowCliPasswd');
  var pwd_input = document.getElementById('pwd_input');
  if (pwd_input.type !== "password") {
    pwd_input.type = "password";
    bouton.src="images/oeuil.svg"
    bouton.title="Show password"
  }

  ssid_input.value = SSID + " (ch." + channel + ")";
  pwd_input.value  = ""
  connectOK = true;
  suiteOK = false;

  // Affiche la boite de dialogue de saisie du mot de passe réseau
  afficheDialog('dlgConnect');
  // Active les boutons de la boite de dialogue
  // (ils ont peut être été désactivé par une connexion précédente)
  btnConnect.disabled = false;
  btnAnnuler.disabled = false;
  // Donne le focus au champ de saisie du mot de passe
  pwd_input.focus();

  // Attente click sur bouton Connexion
  while (!suiteOK) {
    await sleep(100);
  }
  suiteOK = false;
  
  if (connectOK) {
    pwd = pwd_input.value;
    // Désactive les boutons de la boite de dialogue
    btnConnect.disabled = true;
    btnAnnuler.disabled = true;
    // Affiche l'annimation d'attente
    divWait.classList.remove("noshow");
    // Lance la connexion de la station
    //alert("XMLHttpRequest_post_wificonnect(" + SSID + ", " + pwd + ", " + channel + ");");
    XMLHttpRequest_post_wificonnect(SSID, pwd, channel);
    // On fermera la boite de dialogue et on masquera
    // l'animation d'attente dans 10 secondes
    setTimeout(function() { divWait.classList.add("noshow"); }, 10000);
    setTimeout(function() { closeDialog('dlgConnect'); }, 10000);
  } else {
    //alert("Canceled connection.");
    closeDialog('dlgConnect');
    btnConnect.disabled = false;
    btnAnnuler.disabled = false;
    pwd_input.value = "";
  }

}

function afficheDialog(dialog_id) {
  // Affiche la boite de dialogue
  var dlgMask = document.getElementById("dlgMask0");
  var dlgBox  = document.getElementById(dialog_id);
  
  dlgMask.classList.remove("noshow");
  window.setTimeout(function () {
    dlgBox.classList.remove("masquer");
  }, 0.2);
}

function closeDialog(dialog_id) {
  // Ferme la boite de dialogue
  var dlgMask = document.getElementById("dlgMask0");
  var dlgBox  = document.getElementById(dialog_id);
  dlgBox.classList.add("masquer");
  window.setTimeout(function () {
    dlgMask.classList.add("noshow");
  }, 1);
}

function connect_clique() {
  connectOK = true;
  suiteOK   = true;
  // Refresh wifi status
  get_networks();
}

function connect_cancel() {
  connectOK = false;
  suiteOK   = true;
}

function connect_keyup(e) {
  if (e.keyCode === 13) {
    document.getElementById("btnConnect").click();
  } else if (e.keyCode === 27) {
    document.getElementById("btnAnnuler").click();
  }
}

function XMLHttpRequest_post_wificonnect(SSID, pwd, channel) {
  var xhttp = new XMLHttpRequest();

  xhttp.onreadystatechange = function() {
    if (xhttp.readyState == 4) {
      if ((xhttp.status == 200) || (xhttp.status == 0)) {
        XMLHttpResult("/wificonnect", xhttp.responseXML, xhttp.responseText);
      } else {
        alert("XMLHttpRequest_post_wificonnect() : Error " + xhttp.status);
      }
    }
  };

  var ssid_encode = encodeURIComponent(SSID);
  var pwd_encode  = encodeURIComponent(pwd);
  if (location.protocol == 'file:') {
    xhttp.open("POST", netDevURL + "/wificonnect", true);
  } else {
    xhttp.open("POST", "/wificonnect", true);
  }
  xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
  xhttp.send("ssid=" + ssid_encode + "&pwd=" + pwd_encode + "&channel=" + channel);

}

function deconnect_clique() {
  var message = "";
  message = "Do you really want to disconnect from the current network\nand clear the client WiFi settings in the balance?";
  if (confirm(message)) {
    XMLHttpRequest_get("deconnexion");
  }
  // Refresh wifi status
  get_networks();
}

function toogleShowPasswd(dest) {
  if (dest == "cli") {
    const bouton = document.getElementById('btnShowCliPasswd');
    var pwd_input = document.getElementById('pwd_input');
    if (pwd_input.type === "password") {
      pwd_input.type = "text";
      bouton.src="images/oeuil-barre.svg"
      bouton.title="Hide password"
    } else {
      pwd_input.type = "password";
      bouton.src="images/oeuil.svg"
      bouton.title="Show password"
    }
  }
}
