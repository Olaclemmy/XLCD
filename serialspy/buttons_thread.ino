/* ===============================================
 * Project: XLCD, Button handling
 * ===============================================
 * Autor:     Frank (xpix) Herrmann
 * Email:     xpixer@gmail.com
 * License:   all Free
 * Last edit: 30.08.2013
 */ 

/*--------------------------------------------------------------------------------------
  ReadButton()
  Detect the button pressed and return the value
  Uses global values buttonWas, buttonJustPressed, buttonJustReleased.
--------------------------------------------------------------------------------------*/
byte ReadButton()
{
   // return no button pressed if the below checks don't write to btn
   byte button = BUTTON_NONE;   
   
   //read the button ADC pin voltage first on A then on B
   char buttonrow = 'A';
   unsigned int buttonVoltage = analogRead( BUTTONS_A_ADC_PIN );
   if(buttonVoltage <= 5 + BUTTONHYSTERESIS){
      buttonrow = 'B';
      buttonVoltage = analogRead( BUTTONS_B_ADC_PIN );
   }
   if(buttonVoltage <= 5 + BUTTONHYSTERESIS){
      return button;
   }

   // test all values to discover button
   for (int i = 0; i < 7 ; i = i + 1) {
      if( buttonVoltage < ( button_power[i] + BUTTONHYSTERESIS ) )
      {
         button = i;
      }
   }   

   if( buttonrow == 'B'){
      button = button + 10;
   }
   
   //handle button flags for just pressed and just released events
   if( ( buttonWas == BUTTON_NONE ) && ( button != BUTTON_NONE ) )
   {
      //the button was just pressed, set buttonJustPressed, this can optionally be used to trigger a once-off action for a button press event
      //it's the duty of the receiver to clear these flags if it wants to detect a new button change event
      buttonJustPressed  = true;
      buttonJustReleased = false;
   }
   if( ( buttonWas != BUTTON_NONE ) && ( button == BUTTON_NONE ) )
   {
      buttonJustPressed  = false;
      buttonJustReleased = true;
   }

   //save the latest button value, for change event detection next time round
   buttonWas = button;   
   
   return( button );
}   