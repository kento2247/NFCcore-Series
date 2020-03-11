/*関数一覧
  String nfc_read_name()
  bool nfc_write_name(String FirstName, String LastName)
*/





String nfc_read_name() {
  Serial.println(F("NFC begin read user_name data"));
  Serial.println(F(" use block: 2"));
  byte nfc_nameData[18];
  String returnString;   //呼び出し元に返送するための変数を生成
  if (nfc_read(nfc_nameData, 2) == true) {
    Serial.print(F(" raw data = "));
    for (byte i = 0; i < 16; i++) {
      Serial.print(nfc_nameData[i]);
      returnString += char(nfc_nameData[i]); //1文字ずつ変換して、文字列に付け加えていく
      if (nfc_nameData[i] == 0) break;       //変換完了
      Serial.print(F(":"));
    }
    Serial.println();
    Serial.print(F("result: "));
    Serial.println(returnString);
    Serial.println();
    return returnString;  //変換したデータを呼び出し元に返送する
  }
  else {
    Serial.println(F("nfc_read_name() failed"));
    Serial.println();
    return String (F("failed"));
  }
}



bool nfc_write_name(String FirstName, String LastName) {
  Serial.println(F("NFC begin write user_name data"));
  String Name = FirstName + "-" + LastName;
  Serial.print(Name);

  if (Name.length() >= 16) {
    Serial.println(F("nfc_write_name() failed (name data is too long)"));
    return false;
  }
  else {
    Serial.println(F(">>convert to ASCIIcode"));
    byte nfc_writeData[16]= {
      0,0,0,0,
      0,0,0,0,
      0,0,0,0,
      0,0,0,0
      };
    Name.getBytes(nfc_writeData, 16);  //NAME変数をASCIIコード配列に変換
    if (!nfc_write(nfc_writeData, 2)) {
      Serial.println(F("nfc_write_name() failed"));
      Serial.println();
      return false;
    }
    else {
      Serial.println(F("nfc_write_name() is in success"));
      Serial.println();
      return true;
    }
  }
}
bool nfc_version_check() {
  byte sector0_data[18];
  if (!nfc_read(sector0_data, 1)) return false;
  if (sector0_data[0] == NFCCore_version) {
    Serial.println(F("This  is the latest version"));
    Serial.println();
    return true;
  }
  else {
    bool Update;
    Serial.println(F("This is not the latest version"));
    Serial.println(F("Enter 1 if you want to update, enter 0 to end"));
    Serial.print(F("command >> "));
    while (Serial.available()) Serial.read();  //シリアルポートに溜まっているいらない情報を捨てる
    while (!Serial.available() > 0);  //pcからのシリアル信号（コマンド入力）を待つ
    while (Serial.available()) Update = (byte)(Serial.readStringUntil("¥n").toInt());  //読んだ信号をint型の数字に変換
    Serial.println(Update);
    Serial.println()
      Seril.println(F("Please,touch your card again."))
      while (!nfc_check());

  }
    if (Update && nfc_version_update(sector0_data)) return true;
    if (!Update) return true;
    else return false;
  }
  
}





bool nfc_version_update(byte WriteData[]) {
  Serial.println(F("NFC begin card version update"));
  WriteData[0] = NFCCore_version;
  if (!nfc_write(WriteData, 1)) {
    Serial.println(F("nfc_version_update failed"));
    return false;
  }
  else {
    Serial.println(F("nfc_version_update is in success"));
    return true;
  }
  Serial.println();
}
