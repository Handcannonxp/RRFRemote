// Copyright (c) F4HWN Armel. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// View histogram
void viewHistogram(uint16_t maxLevel, uint16_t tx[]) {
  const char *legende[] = {"00", "06", "12", "18", "23"};
  uint8_t i, j, k, tmp, delta;

  j = 6;
  k = 0;

  for (i = 0; i < 24; i++)
  {
    if (tx[i] != 0)
    {
      tmp = map(tx[i], 0, maxLevel, 0, 34);
      M5.Lcd.fillRect(j, 139 - tmp, 5, tmp, TFT_FRONT);
      M5.Lcd.fillRect(j, 139 - 34, 5, 34 - tmp, TFT_BACK);
    }
    else
    {
      M5.Lcd.fillRect(j, 139 - 34, 5, 34, TFT_BACK);
    }
    M5.Lcd.fillRect(j, 140, 5, 1, TFT_LIGHTGREY);
    j += 6;
    k += 2;
  }

  M5.Lcd.setFreeFont(0);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BACK);
  M5.Lcd.setTextDatum(CL_DATUM);
  M5.Lcd.setTextPadding(0);

  for (i = 0; i < 5; i++)
  {
    scroll(20);

    delta = String(legende[i]).toInt();

    delta += utc - 1 - dst;
    delta = (delta < 00) ? 24 + delta : delta;
    delta = (delta > 23) ? delta - 24 : delta;

    if(delta < 10)
    {
      M5.Lcd.drawString("0" + String(delta), 4 + (i * 34), 148);
    }
    else
    {
      M5.Lcd.drawString(String(delta), 4 + (i * 34), 148);
    }
  }
}

// View elsewhere
void viewElsewhere(DynamicJsonDocument doc, const char *salon) {
  const char *elsewhere = "";
  uint16_t linkTotalElsewhere = 0;
  uint8_t i, j;

  String left[10];
  String middle[10];
  String right[10];
  static String leftOld[10];
  static String middleOld[10];
  static String rightOld[10];

  JsonObject obj = doc.as<JsonObject>();
  elsewhereString = obj["elsewhere"].as<String>();

  scroll(20);

  M5.Lcd.setFreeFont(&tahoma6pt7b);

  size_t stop = sizeof(room) / sizeof(room[0]);
  stop -= 1;

  if(reset == 0) {
    M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(26);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(leftOld[i].substring(0, 3), 14, 160 + (14 * i));
    }

    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(70);

    for(i = 0; i < stop; i++) {
      if(strstr(middleOld[i].c_str(), "LINK") != NULL) {
        M5.Lcd.fillRect(28, 155 + (14 * i), 72, 13, TFT_WHITE);
        M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
      }
      else {
        M5.Lcd.fillRect(28, 155 + (14 * i), 72, 13, TFT_FRONT);
        M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
      }
      Serial.println(middleOld[i]);
      M5.Lcd.drawString(middleOld[i], 64, 160 + (14 * i));
    }
    
    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(52);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(rightOld[i], 128, 160 + (14 * i));
    }
  }

  if(elsewhereString != "null") {
    if (elsewhereString != elsewhereStringOld || refresh == 0)
    {
      elsewhereStringOld = elsewhereString;

      j = 0;
      for(i = 0; i <= stop; i++)
      {
        scroll(20);
        if (strcmp(room[i], salon) != 0)
        {
          left[j] = String(room[i]).substring(0, 3);
          elsewhere = doc["elsewhere"][1][room[i]];

          //elsewhere = "GW-ALLSTAR-40020";
          //elsewhere = "(67) F1ZRZ T10M";
          //elsewhere = "(67) F1ZRZ S";
          //elsewhere = "(76) TM100SHT H";

          if (strcmp(elsewhere, "Aucune émission") != 0)
          {
            screensaver = millis(); // Screensaver update !!!
            tmpString = String(elsewhere);
            if(tmpString.substring(0, 3) == "GW-") {
              tmpString = "GATEWAY";
            }
            else {
              tmpString = getValue(elsewhere, ' ', 1);
              tmpString = (tmpString == "") ? "RTFM" : getValue(elsewhere, ' ', 1) + ' ' + getValue(elsewhere, ' ', 2);
            }
          }
          else
          {
            linkTotalElsewhere = doc["elsewhere"][5][room[i]];
            sprintf(swap, "%d", linkTotalElsewhere);
            tmpString = swap;
            tmpString += " LINK";
            if(linkTotalElsewhere > 1)
            {
              tmpString += "S";
            }
          }

          middle[j] = tmpString;
          elsewhere = doc["elsewhere"][3][room[i]];
          right[j] = String(elsewhere);
          j += 1;
        }
      }

      M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
      M5.Lcd.setTextDatum(CC_DATUM);
      M5.Lcd.setTextPadding(26);

      for(i = 0; i < stop; i++) {
        if(left[i] != leftOld[i]) {
          leftOld[i] = left[i];
          M5.Lcd.drawString(left[i].substring(0, 3), 14, 160 + (14 * i));
        }
      }

      M5.Lcd.setTextDatum(CC_DATUM);
      M5.Lcd.setTextPadding(70);

      for(i = 0; i < stop; i++) {
        if(middle[i] != middleOld[i]) {
          middleOld[i] = middle[i];
          if(strstr(middle[i].c_str(), "LINK") != NULL) {
            M5.Lcd.fillRect(28, 155 + (14 * i), 72, 13, TFT_WHITE);
            M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
          }
          else {
            M5.Lcd.fillRect(28, 155 + (14 * i), 72, 13, TFT_FRONT);
            M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
          }
          M5.Lcd.drawString(middle[i], 64, 160 + (14 * i));
        }
      }

      M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
      M5.Lcd.setTextDatum(CC_DATUM);
      M5.Lcd.setTextPadding(52);

      for(i = 0; i < stop; i++) {
        if(right[i] != rightOld[i]) {
          rightOld[i] = right[i];
          M5.Lcd.drawString(right[i], 128, 160 + (14 * i));
        }
      }
    }
  }
}

// View settings
void viewSettings() {
  uint8_t i, j;
  String data[10];
  static String dataOld[10];

  if (refresh == 0)
  {
    M5.Lcd.setTextPadding(160);
    M5.Lcd.drawString("RRFRemote " + String(VERSION), 240, 108);

    i = 161;
    j = 74;
    M5.Lcd.fillRoundRect(160, 117, 160, 122, 4, TFT_WHITE);
    M5.Lcd.fillRoundRect(i, 118, j, 120, 4, TFT_FRONT);
    M5.Lcd.drawFastVLine(i + j - 3, 118, 120, TFT_BACK);
    M5.Lcd.drawFastVLine(i + j - 2, 118, 120, TFT_WHITE);
    M5.Lcd.drawFastVLine(i + j - 1, 118, 120, TFT_WHITE);

    String system[] = {"CPU", "CPU Cores", "CPU Freq", "Chip Rev", "Flash Speed", "Flash Size", "Free RAM", "Free Heap", "IP", "Battery"};

    for (i = 0; i < 10; i++)
    {
      M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
      M5.Lcd.setTextDatum(CL_DATUM);
      M5.Lcd.setTextPadding(0);
      M5.Lcd.drawString(system[i], 163, 122 + (12 * i));

      M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
      M5.Lcd.setTextDatum(CR_DATUM);
      M5.Lcd.setTextPadding(70);
      M5.Lcd.drawString(dataOld[i], 318, 122 + (12 * i));
    }

    refresh = 1;
  }

  data[0] = "ESP32";
  data[1] = "2";
  data[2] = String(ESP.getCpuFreqMHz()) + " Mhz";
  data[3] = ESP.getChipRevision();
  data[4] = String(ESP.getFlashChipSpeed() / 1000000) + " Mhz";
  data[5] = String(ESP.getFlashChipSize() / (1024 * 1024)) + " Mb";
  data[6] = String(ESP.getFreeHeap() / 1024) + " kb";
  data[7] = String(esp_get_minimum_free_heap_size() / 1024) + " kb";
  data[8] = WiFi.localIP().toString();
  data[9] = String(getBatteryLevel(1)) + " %";

  M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Lcd.setTextDatum(CR_DATUM);
  M5.Lcd.setTextPadding(70);

  for(i = 0; i < 10; i++) {
    if(data[i] != dataOld[i]) {
      dataOld[i] = data[i];
      M5.Lcd.drawString(data[i], 318, 122 + (12 * i));
    }
  }
}

// View propagation
void viewPropagation() {
  uint8_t i, j;
  int16_t parenthesisBegin = 0;
  int16_t parenthesisLast = 0;
  String data[10];
  static String dataOld[10];

  if (refresh == 0)
  {
    M5.Lcd.setTextPadding(160);
    M5.Lcd.drawString("Propagation", 240, 108);

    i = 161;
    j = 70;
    M5.Lcd.fillRoundRect(160, 117, 160, 122, 4, TFT_WHITE);
    M5.Lcd.fillRoundRect(i, 118, j, 120, 4, TFT_FRONT);
    M5.Lcd.drawFastVLine(i + j - 3, 118, 120, TFT_BACK);
    M5.Lcd.drawFastVLine(i + j - 2, 118, 120, TFT_WHITE);
    M5.Lcd.drawFastVLine(i + j - 1, 118, 120, TFT_WHITE);

    M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
    M5.Lcd.setTextDatum(CL_DATUM);
    M5.Lcd.setTextPadding(0);

    String solar[] = {"SFI", "Sunspots", "A-Index", "K-Index", "X-Ray", "Ptn Flux", "Elc Flux", "Aurora", "Solar Wind", "Update"};

    for (i = 0; i < 10; i++)
    {
      M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
      M5.Lcd.setTextDatum(CL_DATUM);
      M5.Lcd.setTextPadding(0);
      M5.Lcd.drawString(solar[i], 163, 122 + (12 * i));

      M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
      M5.Lcd.setTextDatum(CR_DATUM);
      M5.Lcd.setTextPadding(70);
      M5.Lcd.drawString(dataOld[i], 318, 122 + (12 * i));
    }

    refresh = 1;
  }

  String solar[] = {"solarflux", "sunspots", "aindex", "kindex", "xray", "protonflux", "electonflux", "aurora", "solarwind"};

  for (i = 0; i < 9; i++)
  {
    tmpString = xmlData;
    tmpString.replace("<" + solar[i] + ">", "(");
    tmpString.replace("</" + solar[i] + ">", ")");
    parenthesisBegin = tmpString.indexOf("(");
    parenthesisLast = tmpString.indexOf(")");
    if (parenthesisBegin > 0)
    {
      tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
    }
   data[i] = tmpString;
  }

  tmpString = xmlData;
  tmpString.replace("<updated>", "(");
  tmpString.replace("</updated>", ")");
  parenthesisBegin = tmpString.indexOf("(");
  parenthesisLast = tmpString.indexOf(")");
  if (parenthesisBegin > 0)
  {
    tmpString = tmpString.substring(parenthesisBegin + 13, parenthesisLast);
  }
  data[9] = tmpString.substring(1, 3) + ":" + tmpString.substring(3);

  M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Lcd.setTextDatum(CR_DATUM);
  M5.Lcd.setTextPadding(70);

  for(i = 0; i < 10; i++) {
    if(data[i] != dataOld[i]) {
      dataOld[i] = data[i];
      M5.Lcd.drawString(data[i], 318, 122 + (12 * i));
    }
  }
}

// View Top Links
void viewTopLinks(uint8_t stop, uint16_t allTx[10], const char *allIndicatif[10], const char *allDuree[10], const char *salon) {
  uint8_t i, j;
  String left[10];
  String middle[10];
  String right[10];
  static String leftOld[10];
  static String middleOld[10];
  static String rightOld[10];

  if (refresh == 0)
  {
    M5.Lcd.setTextPadding(160);
    M5.Lcd.drawString("Top Links sur " + String(salon).substring(0, 3), 240, 108);

    i = 161;
    j = 38;
    M5.Lcd.fillRoundRect(160, 117, 160, 122, 4, TFT_WHITE);
    M5.Lcd.fillRoundRect(i, 118, j, 120, 4, TFT_FRONT);
    M5.Lcd.drawFastVLine(i + j - 3, 118, 120, TFT_BACK);
    M5.Lcd.drawFastVLine(i + j - 2, 118, 120, TFT_WHITE);
    M5.Lcd.drawFastVLine(i + j - 1, 118, 120, TFT_WHITE);

    M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
    M5.Lcd.setTextDatum(CL_DATUM);
    M5.Lcd.setTextPadding(0);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(leftOld[i], 163, 122 + (12 * i));
    }

    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(70);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(middleOld[i], 234, 122 + (12 * i));
    }

    M5.Lcd.setTextDatum(CR_DATUM);
    M5.Lcd.setTextPadding(0);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(rightOld[i], 318, 122 + (12 * i));
    }

    refresh = 1;
  }

  for (i = 0; i < stop; i++)
  {
    if(allTx[i] > 99)
    {
      allTx[i] = 99;
    }

    sprintf(swap, "%02d", allTx[i]);
    tmpString = swap;
    tmpString += " TX";

    left[i] = tmpString;

    tmpString = String(allIndicatif[i]);
    if(tmpString.substring(0, 3) == "GW-") {
      tmpString = "GATEWAY";
    }
    else {
      tmpString = getValue(allIndicatif[i], ' ', 1);
      tmpString = (tmpString == "") ? "RTFM" : getValue(allIndicatif[i], ' ', 1) + ' ' + getValue(allIndicatif[i], ' ', 2);
    }

    middle[i] = tmpString;
    right[i] = String(allDuree[i]);
  }

  M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
  M5.Lcd.setTextDatum(CL_DATUM);
  M5.Lcd.setTextPadding(0);

  for(i = 0; i < stop; i++) {
    if(left[i] != leftOld[i]) {
      leftOld[i] = left[i];
      M5.Lcd.drawString(left[i], 163, 122 + (12 * i));
    }
  }

  M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(70);

  for(i = 0; i < stop; i++) {
    if(middle[i] != middleOld[i]) {
      middleOld[i] = middle[i];
      M5.Lcd.drawString(middle[i], 234, 122 + (12 * i));
    }
  }

  M5.Lcd.setTextDatum(CR_DATUM);
  M5.Lcd.setTextPadding(0);

  for(i = 0; i < stop; i++) {
    if(right[i] != rightOld[i]) {
      rightOld[i] = right[i];
      M5.Lcd.drawString(right[i], 318, 122 + (12 * i));
    }
  }
}

// View blocages

void viewBlocage(uint8_t stop, const char *iptableIndicatif[10], const char *iptableType[10], const char *salon) {
  uint8_t i, j;
  int16_t parenthesisBegin = 0;
  int16_t parenthesisLast = 0;
  String left[10];
  String right[10];
  static String leftOld[10];
  static String rightOld[10];

  if(stop > 9) {
    stop = 9;
  }

  if (refresh == 0)
  {
    M5.Lcd.setTextPadding(160);
    M5.Lcd.drawString("Blocages sur " + String(salon).substring(0, 3), 240, 108);

    i = 161;
    j = 50;

    for (uint8_t k = 0; k < stop; k++) {
      tmpString = String(iptableType[k]);
      if(tmpString == "INTEMPESTIF") {
        j = 80;
        break;
      }
    }

    M5.Lcd.fillRoundRect(160, 117, 160, 122, 4, TFT_WHITE);
    M5.Lcd.fillRoundRect(i, 118, j, 120, 4, TFT_FRONT);
    M5.Lcd.drawFastVLine(i + j - 3, 118, 120, TFT_BACK);
    M5.Lcd.drawFastVLine(i + j - 2, 118, 120, TFT_WHITE);
    M5.Lcd.drawFastVLine(i + j - 1, 118, 120, TFT_WHITE);

    M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
    M5.Lcd.setTextDatum(CL_DATUM);
    M5.Lcd.setTextPadding(0);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(leftOld[i], 163, 122 + (12 * i));
    }

    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
    M5.Lcd.setTextDatum(CR_DATUM);
    M5.Lcd.setTextPadding(74);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(rightOld[i], 318, 122 + (12 * i));
    }

    refresh = 1;
  }

  for (uint8_t i = 0; i < stop; i++)
  {
    tmpString = String(iptableType[i]);

    parenthesisBegin = tmpString.indexOf('(');
    parenthesisLast = tmpString.indexOf(')');
    if (parenthesisBegin > 0)
    {
      tmpString = tmpString.substring(parenthesisBegin + 1, parenthesisLast);
    }

    left[i] = tmpString;

    tmpString = String(iptableIndicatif[i]);
    if(tmpString.substring(0, 3) == "GW-") {
      tmpString = "GATEWAY";
    }
    else {
      tmpString = getValue(iptableIndicatif[i], ' ', 1);
      tmpString = (tmpString == "") ? "RTFM" : getValue(iptableIndicatif[i], ' ', 1) + ' ' + getValue(iptableIndicatif[i], ' ', 2);
    }

    right[i] = tmpString;
  }

  M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
  M5.Lcd.setTextDatum(CL_DATUM);
  M5.Lcd.setTextPadding(0);

  for(i = 0; i < stop; i++) {
    if(left[i] != leftOld[i]) {
      leftOld[i] = left[i];
      M5.Lcd.drawString(left[i], 163, 122 + (12 * i));
    }
  }

  M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Lcd.setTextDatum(CR_DATUM);
  M5.Lcd.setTextPadding(74);

  for(i = 0; i < stop; i++) {
    if(right[i] != rightOld[i]) {
      rightOld[i] = right[i];
      M5.Lcd.drawString(right[i], 318, 122 + (12 * i));
    }
  }
}

// View ISS
void viewISS(StaticJsonDocument<512> docISS) {
  uint8_t i, j;
  const char *issDataString;
  float issDataNum;
  String data[10];
  static String dataOld[10];

  if (refresh == 0)
  {
    M5.Lcd.setTextPadding(160);
    M5.Lcd.drawString("ISS", 240, 108);

    i = 161;
    j = 70;
    M5.Lcd.fillRoundRect(160, 117, 160, 122, 4, TFT_WHITE);
    M5.Lcd.fillRoundRect(i, 118, j, 120, 4, TFT_FRONT);
    M5.Lcd.drawFastVLine(i + j - 3, 118, 120, TFT_BACK);
    M5.Lcd.drawFastVLine(i + j - 2, 118, 120, TFT_WHITE);
    M5.Lcd.drawFastVLine(i + j - 1, 118, 120, TFT_WHITE);

    String iss[] = {"Distance", "Units", "Visibility", "Latitude", "Longitude", "Altitude", "Velocity", "Footprint", "Solar Lat", "Solar Long"};

    for (i = 0; i < 10; i++)
    {
      M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
      M5.Lcd.setTextDatum(CL_DATUM);
      M5.Lcd.setTextPadding(0);
      M5.Lcd.drawString(iss[i], 163, 122 + (12 * i));

      M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
      M5.Lcd.setTextDatum(CR_DATUM);
      M5.Lcd.setTextPadding(70);
      M5.Lcd.drawString(dataOld[i], 318, 122 + (12 * i));
    }

    refresh = 1;
  }

  String iss[] = {"distance", "units", "visibility", "latitude", "longitude", "altitude", "velocity",  "footprint", "solar_lat", "solar_lon"};

  DeserializationError error = deserializeJson(docISS, issData); // Deserialize the JSON document
  if (!error) // And no error
  {
    for (i = 0; i < 10; i++)
    {
      if(i == 0) {
        issDistance = computeDistance((float)(docISS["latitude"]), (float)(docISS["longitude"]));
        data[i] = String(issDistance);
      }
      else if (i > 0 && i < 3) 
      { 
        issDataString = docISS[iss[i]];
        data[i] = String(issDataString);
        //tmpString[0] = toupper(tmpString[0]);
      }
      else 
      {
        issDataNum = docISS[iss[i]];
        data[i] = String(issDataNum);
      }
    }
  }

  M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Lcd.setTextDatum(CR_DATUM);
  M5.Lcd.setTextPadding(70);

  for(i = 0; i < 10; i++) {
    if(data[i] != dataOld[i]) {
      dataOld[i] = data[i];
      M5.Lcd.drawString(data[i], 318, 122 + (12 * i));
    }
  }
}

// View last links
void viewLastLinks(uint8_t stop, const char *lastHeure[10], const char *lastIndicatif[10], const char *lastDuree[10], const char *salon) {
  uint8_t i, j;
  String left[10];
  String middle[10];
  String right[10];
  static String leftOld[10];
  static String middleOld[10];
  static String rightOld[10];

  if (refresh == 0)
  {
    M5.Lcd.setTextPadding(160);
    M5.Lcd.drawString("Derniers TX sur " + String(salon).substring(0, 3), 240, 108);

    i = 161;
    j = 38;
    M5.Lcd.fillRoundRect(160, 117, 160, 122, 4, TFT_WHITE);
    M5.Lcd.fillRoundRect(i, 118, j, 120, 4, TFT_FRONT);
    M5.Lcd.drawFastVLine(i + j - 3, 118, 120, TFT_BACK);
    M5.Lcd.drawFastVLine(i + j - 2, 118, 120, TFT_WHITE);
    M5.Lcd.drawFastVLine(i + j - 1, 118, 120, TFT_WHITE);

    M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
    M5.Lcd.setTextDatum(CL_DATUM);
    M5.Lcd.setTextPadding(0);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(leftOld[i], 163, 122 + (12 * i));
    }

    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(80);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(middleOld[i], 242, 122 + (12 * i));
    }

    M5.Lcd.setTextDatum(CR_DATUM);
    M5.Lcd.setTextPadding(0);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(rightOld[i], 318, 122 + (12 * i));
    }

    refresh = 1;
  }

  for (i = 0; i < stop; i++)
  {
    tmpString = String(lastHeure[i]);
    tmpString = tmpString.substring(0, 5);

    int8_t delta = tmpString.substring(0, 2).toInt();
    
    delta += utc - 1 - dst;
    delta = (delta < 00) ? 24 + delta : delta;
    delta = (delta > 23) ? delta - 24 : delta;

    if(delta < 10)
    {
      tmpString = "0" + String(delta) + tmpString.substring(2, 5);
    }
    else
    {
      tmpString = String(delta) + tmpString.substring(2, 5);
    }

    left[i] = tmpString;

    tmpString = String(lastIndicatif[i]);
    if(tmpString.substring(0, 3) == "GW-") {
      tmpString = "GATEWAY";
    }
    else {
      tmpString = getValue(lastIndicatif[i], ' ', 1);
      tmpString = (tmpString == "") ? "RTFM" : getValue(lastIndicatif[i], ' ', 1) + ' ' + getValue(lastIndicatif[i], ' ', 2);
    }

    middle[i] = tmpString;
    right[i] = String(lastDuree[i]);
  }

  M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
  M5.Lcd.setTextDatum(CL_DATUM);
  M5.Lcd.setTextPadding(0);

  for(i = 0; i < stop; i++) {
    if(left[i] != leftOld[i]) {
      leftOld[i] = left[i];
      M5.Lcd.drawString(left[i], 163, 122 + (12 * i));
    }
  }

  M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(80);

  for(i = 0; i < stop; i++) {
    if(middle[i] != middleOld[i]) {
      middleOld[i] = middle[i];
      M5.Lcd.drawString(middle[i], 242, 122 + (12 * i));
    }
  }

  M5.Lcd.setTextDatum(CR_DATUM);
  M5.Lcd.setTextPadding(0);

  for(i = 0; i < stop; i++) {
    if(right[i] != rightOld[i]) {
      rightOld[i] = right[i];
      M5.Lcd.drawString(right[i], 318, 122 + (12 * i));
    }
  }
}

// View data
String viewData(uint8_t icon, String data, String dataOld) {
  if (dataOld != data)
  {
    M5.Lcd.fillRect(4, 2, 36, 42, TFT_HEADER);
    //Serial.println(data);

    sprintf(swap, "%c", icon);
    tmpString = swap;
    M5.Lcd.drawString(tmpString, 10, 20);

    M5.Lcd.setFreeFont(&rounded_led_board10pt7b);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_INFO);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(320);
    M5.Lcd.drawString(data, 160, 60);

    dateStringOld = "";
    linkTotalStringOld = "";
    linkActifStringOld = "";
    txTotalStringOld = "";
    emissionStringOld = "";
    issStringOld = "";

    return data;
  }
  return dataOld;
}

// view menu
void viewMenu() {
  if (menuRefresh == 0)
  {
    M5.Lcd.setBrightness(brightnessCurrent);

    M5.Lcd.fillRect(4, 4, 316, 40, TFT_HEADER);

    M5.Lcd.setFreeFont(ICON_FONT);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_HEADER);
    M5.Lcd.setTextDatum(CL_DATUM);
    M5.Lcd.setTextPadding(40);
    sprintf(swap, "%c", ICON_LEFT);
    tmpString = swap;
    M5.Lcd.drawString(tmpString, 10, 18);

    M5.Lcd.setFreeFont(ICON_FONT);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_HEADER);
    M5.Lcd.setTextDatum(CR_DATUM);
    M5.Lcd.setTextPadding(40);
    sprintf(swap, "%c", ICON_RIGHT);
    tmpString = swap;
    M5.Lcd.drawString(tmpString, 310, 18);

    titleStringOld = "";
    optionStringOld = "";

    menuRefresh = 1;
  }
  
  M5.Lcd.setFreeFont(&dot15pt7b);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_HEADER);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(220);

  titleString = "";
  optionString = String(menu[menuCurrent]);

  if (menuSelected == -1)
  {
    titleString = "MODE MENU";
  }
  else
  {
    titleString = String(menu[menuSelected]);

    if(optionString == "SYSOP") 
    {
      optionString = String(sysop[sysopCurrent]);
    }
    else if(optionString == "COULEUR") 
    {
      optionString = String(color[colorCurrent]);
    }
    else if(optionString == "QSY") 
    {
      optionString = String(room[roomCurrent]);
    }
    else if(optionString == "LUMINOSITE") 
    {
      optionString = "LEVEL " + String(brightnessCurrent);      
    }
    else if(optionString == "CONFIG") 
    {
      optionString = String(config[(configCurrent * 6) + 4]);   
    }
    else if(optionString == "RAPTOR") 
    {
      optionString = (raptorCurrent == 0) ? "RAPTOR OFF" : "RAPTOR ON";
    }
    else if(optionString == "ISS") 
    {
      optionString = (issCurrent == 0) ? "ISS OFF" : "ISS ON";
    }
    else if(optionString == "TOT") 
    {
      optionString = (totCurrent == 0) ? "TOT OFF" : "TOT ON";
    }
    else if(optionString == "MODE") 
    {
    optionString = (modeCurrent == 0) ? "MODE EXPERT" : "MODE BASIC";
    }
    else if(optionString == "FOLLOW") 
    {
      optionString = (followCurrent == 0) ? "FOLLOW OFF" : "FOLLOW ON";
    }
  }

  if(titleString != titleStringOld) 
  {
    titleStringOld = titleString;
    M5.Lcd.drawString(titleString, 160, 16);
  }

  if(optionString != optionStringOld)
  {
    optionStringOld = optionString;
    M5.Lcd.setFreeFont(&rounded_led_board10pt7b);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_INFO);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(320);
    M5.Lcd.drawString(optionString, 160, 60);
  }
}

// View baseline
void viewBaseline() {
  M5.Lcd.setFreeFont(0);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_HEADER);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(220);

  baselineString = config[(configCurrent * 6) + 4];

  if ((String)config[(configCurrent * 6) + 5] != "")
  {
    if (dtmf[roomCurrent] != whereisCurrent && followCurrent == 0)
    {
      baselineString += " / " + whereisString;
    }

    if(raptorCurrent == 1) {
      baselineString += " / RAPTOR ON" ;
    }

    if(followCurrent == 1) {
      baselineString += " / FOLLOW ON";
    }
  }

  M5.Lcd.drawString(baselineString, 160, 36);
  M5.Lcd.drawFastHLine(0, 0, 320, TFT_WHITE);
}

// View battery
void viewBattery()
{
  if(reset == 0 || batteryLevelCurrent != getBatteryLevel(0) || batteryChargeCurrent != isCharging())
  {
    batteryLevelCurrent = getBatteryLevel(0);
    batteryChargeCurrent = isCharging();

    M5.Lcd.setFreeFont(&Battery_Icons21pt7b);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_HEADER);
    M5.Lcd.setTextDatum(CR_DATUM);
    M5.Lcd.setTextPadding(0);

    if (isCharging() && screensaverMode == 0)
    {
      sprintf(swap, "%c", ICON_CHARGING);
      tmpString = swap;
      M5.Lcd.drawString(tmpString, 310, 18);
      //M5.Lcd.setBrightness(128);
    }
    else
    {
      switch(getBatteryLevel(0))
      {
        case 100: sprintf(swap, "%c", ICON_BAT100); break;
        case  75: sprintf(swap, "%c", ICON_BAT075); break;
        case  50: sprintf(swap, "%c", ICON_BAT050); break;
        case  25: sprintf(swap, "%c", ICON_BAT025); break;
        default:  sprintf(swap, "%c", ICON_BAT000); break;
      }
      tmpString = swap;
      M5.Lcd.drawString(tmpString, 310, 18);
    }
  }
}

// View elsewhere Big
void viewElsewhereBig(DynamicJsonDocument doc, const char *salon) {
  const char *elsewhere = "";
  uint16_t linkTotalElsewhere = 0;
  uint8_t i, j;

  String left[10];
  String right[10];
  static String leftOld[10];
  static String rightOld[10];

  JsonObject obj = doc.as<JsonObject>();
  elsewhereString = obj["elsewhere"].as<String>();

  scroll(20);

  M5.Lcd.setFreeFont(&tahoma8pt7b);

  size_t stop = sizeof(room) / sizeof(room[0]);
  stop -= 1;

  if(reset == 0) {
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BACK);
    M5.Lcd.setTextPadding(0);
    M5.Lcd.drawString("Trafic en cours", 75, 112);

    M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(45);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(leftOld[i], 25, 134 + (19 * i));
    }

    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(96);

    for(i = 0; i < stop; i++) {
      if(strstr(rightOld[i].c_str(), "LINK") != NULL) {
        M5.Lcd.fillRect(51, 126 + (18 * i) + i, 98, 18, TFT_WHITE);
        M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
      }
      else {
        M5.Lcd.fillRect(51, 126 + (18 * i) + i, 98, 18, TFT_FRONT);
        M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
      }
       M5.Lcd.drawString(rightOld[i], 100, 134 + (19 * i));
    }
  }

  if(elsewhereString != "null") {
    if (elsewhereString != elsewhereStringOld || refresh == 0)
    {
      elsewhereStringOld = elsewhereString;

      j = 0;
      for(i = 0; i <= stop; i++) {
        scroll(20);
        if (strcmp(room[i], salon) != 0)
        {
          left[j] = String(room[i]).substring(0, 3);
          elsewhere = doc["elsewhere"][1][room[i]];

          //elsewhere = "GW-ALLSTAR-40020";
          //elsewhere = "(67) F1ZRZ T10M";
          //elsewhere = "(67) F1ZRZ S";
          //elsewhere = "(76) TM100SHT H";

          if (strcmp(elsewhere, "Aucune émission") != 0)
          {
            screensaver = millis(); // Screensaver update !!!
            tmpString = String(elsewhere);
            if(tmpString.substring(0, 3) == "GW-") {
              tmpString = "GATEWAY";
            }
            else {
              tmpString = getValue(elsewhere, ' ', 1);
              tmpString = (tmpString == "") ? "RTFM" : getValue(elsewhere, ' ', 1) + ' ' + getValue(elsewhere, ' ', 2);
            }
          }
          else
          {
            linkTotalElsewhere = doc["elsewhere"][5][room[i]];
            sprintf(swap, "%d", linkTotalElsewhere);
            tmpString = swap;
            tmpString += " LINK";
            if(linkTotalElsewhere > 1)
            {
              tmpString += "S";
            }
          }

          right[j] = tmpString;
          j += 1;
        }
      }
    }
  }

  M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(45);

  for(i = 0; i < stop; i++) {
    if(left[i] != leftOld[i]) {
      leftOld[i] = left[i];
      M5.Lcd.drawString(left[i].substring(0, 3), 25, 134 + (19 * i));
    }
  }

  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(96);

  for(i = 0; i < stop; i++) {
    if(right[i] != rightOld[i]) {
      rightOld[i] = right[i];
      if(strstr(right[i].c_str(), "LINK") != NULL) {
        M5.Lcd.fillRect(51, 126 + (18 * i) + i, 98, 18, TFT_WHITE);
        M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
      }
      else {
        M5.Lcd.fillRect(51, 126 + (18 * i) + i, 98, 18, TFT_FRONT);
        M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
      }
       M5.Lcd.drawString(right[i], 100, 134 + (19 * i));
    }
  }
}

// View last links Big
void viewLastLinksBig(uint8_t stop, const char *lastHeure[10], const char *lastIndicatif[10], const char *lastDuree[10], const char *salon) {
  uint8_t i;
  String left[10];
  String right[10];
  static String leftOld[10];
  static String rightOld[10];

  scroll(20);

  if(stop > 6) {
    stop = 6;
  }

  M5.Lcd.setFreeFont(&tahoma8pt7b);

  if(reset == 0) {
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BACK);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(150);
    M5.Lcd.drawString("Derniers TX sur " + String(salon).substring(0, 3), 245, 112);

    M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(45);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(leftOld[i], 195, 134 + (19 * i));
    }

    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
    M5.Lcd.setTextDatum(CC_DATUM);
    M5.Lcd.setTextPadding(96);

    for(i = 0; i < stop; i++) {
      M5.Lcd.drawString(rightOld[i], 270, 134 + (19 * i));
    }
  }

  for (i = 0; i < stop; i++)
  {
    tmpString = String(lastHeure[i]);
    tmpString = tmpString.substring(0, 5);

    int8_t delta = tmpString.substring(0, 2).toInt();
    
    delta += utc - 1 - dst;
    delta = (delta < 00) ? 24 + delta : delta;
    delta = (delta > 23) ? delta - 24 : delta;

    if(delta < 10)
    {
      tmpString = "0" + String(delta) + tmpString.substring(2, 5);
    }
    else
    {
      tmpString = String(delta) + tmpString.substring(2, 5);
    }

    left[i] = tmpString;

    tmpString = String(lastIndicatif[i]);
    if(tmpString.substring(0, 3) == "GW-") {
      tmpString = "GATEWAY";
    }
    else {
      tmpString = getValue(lastIndicatif[i], ' ', 1);
      tmpString = (tmpString == "") ? "RTFM" : getValue(lastIndicatif[i], ' ', 1) + ' ' + getValue(lastIndicatif[i], ' ', 2);
    }

    right[i] = tmpString;
  }

  M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(45);

  for(i = 0; i < stop; i++) {
    if(left[i] != leftOld[i]) {
      leftOld[i] = left[i];
      M5.Lcd.drawString(left[i], 195, 134 + (19 * i));
    }
  }

  M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(96);

  for(i = 0; i < stop; i++) {
    if(right[i] != rightOld[i]) {
      rightOld[i] = right[i];
      M5.Lcd.drawString(right[i], 270, 134 + (19 * i));
    }
  }
}

// View DTMF
void viewDTMF() {
  uint8_t i, j, k;

  M5.Lcd.setFreeFont(&tahoma8pt7b);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_FRONT);
  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextPadding(45);

  j = 0;
  k = 0;

  for (i = 0; i < 7; i++) {
    tmpString = String(room[i]);
    tmpString = tmpString.substring(0, 3);
    M5.Lcd.drawString(tmpString, (50 + j), (123 + k));
    j += 110;
    if (i == 2 || i == 5) {
      j = 0;
      k += 48;
    }
  }

  M5.Lcd.drawString("PERROQUET", (50 + j), (123 + k));
  j += 110;
  M5.Lcd.drawString("RAPTOR", (50 + j), (123 + k));
}