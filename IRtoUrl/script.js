// from https://www.hackster.io/BuddyC/wifi-ir-blaster-af6bca#_=_
// Function to send IR commands
function buttonClick(clicked_id){

    if (clicked_id == "irTVpwr"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irTVpwr"} );  
    } 

    if (clicked_id == "irTVsrc"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irTVsrc"} );  
    } 

    if (clicked_id == "irTVmute"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irTVmute"} );  
    } 

    if (clicked_id == "irTVvdn"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irTVvdn"} );  
    } 

	if (clicked_id == "irTVvup"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irTVvup"} );  
    } 
	
    if (clicked_id == "irTVchup"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irTVchup"} );  
    } 

	if (clicked_id == "irTVchdn"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irTVchdn"} );  
    } 
	
    if (clicked_id == "irRECpwr"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irRECpwr"} );  
    } 

    if (clicked_id == "irALLpwr"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irALLpwr"} );  
    } 

	if (clicked_id == "irRECpwrON"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irRECpwrON"} );  
    } 

	if (clicked_id == "irRECpwrOFF"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irRECpwrOFF"} );  
    } 

    if (clicked_id == "irRECmute"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irRECmute"} );  
    } 

    if (clicked_id == "irRECvdn"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irRECvdn"} );  
    } 

	if (clicked_id == "irRECvup"){
        $.get( "curl.php", {
        room: "192.168.1.62", button: "irRECvup"} );  
    } 
 
}