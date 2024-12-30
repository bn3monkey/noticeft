FUNCTION TestNoticeFT()
   LOCAL context AS PTR
   LOCAL ret AS INT

   // Login to the noticeft server
   LOCAL ip := "127.0.0.1"
   LOCAL port := 21
   LOCAL id := "test"
   LOCAL password := "test"

   context := noticeft_login(ip, port, id, password)

   IF context == NIL
      ? "Login failed."
      RETURN
   ENDIF

   // Upload a file
   ret := noticeft_sendFile(context, "./resource/샌주.PNG", "샌주.png")
   ? "Send File Result: " + STR(ret)

   // Check if file exists
   ret := noticeft_exists(context, "샌주.png")
   ? "File Exists Result: " + STR(ret)

   // Create local directory
   DIRECTORY "result" NEW

   // Download the file
   ret := noticeft_receiveFile(context, "샌주.png", "result/샌주.png")
   ? "Receive File Result: " + STR(ret)

   // Remove the file
   ret := noticeft_removeFile(context, "샌주.png")
   ? "Remove File Result: " + STR(ret)

   // Check if file exists after removal
   ret := noticeft_exists(context, "샌주.png")
   ? "File Exists After Removal Result: " + STR(ret)

   // Upload another file
   ret := noticeft_sendFile(context, "./resource/infinity_sans.PNG", "infinity_sans.png")
   ? "Send File Result: " + STR(ret)

   // Rename the file
   ret := noticeft_renameFile(context, "infinity_sans.png", "sans.png")
   ? "Rename File Result: " + STR(ret)

   // Check if old file exists
   ret := noticeft_exists(context, "infinity_sans.png")
   ? "Old File Exists Result: " + STR(ret)

   // Check if new file exists
   ret := noticeft_exists(context, "sans.png")
   ? "New File Exists Result: " + STR(ret)

   // Download the renamed file
   ret := noticeft_receiveFile(context, "sans.png", "result/sans.png")
   ? "Receive Renamed File Result: " + STR(ret)

   // Logout
   noticeft_logout(context)
   ? "Logged out successfully."

   RETURN
ENDFUNCTION