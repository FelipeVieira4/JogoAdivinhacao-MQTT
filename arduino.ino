#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4 
#include <ArduinoJson.h>

#define pinoBuzzer 10

#define col 16 // Serve para definir o numero de colunas do display utilizado
#define lin  2 // Serve para definir o numero de linhas do display utilizado
#define ende  0x27 // Serve para definir o endereço do display.

#define botaoJoystick 4
#define Vx A0

LiquidCrystal_I2C lcd_1(ende,col,lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C

DynamicJsonDocument pacoteDados(1024);

uint8_t dificuldadeJogo,modoJogo;
uint8_t chances,chute,correto;

int placarJogo=0; 

// Mudar o número que o player vai chutar
void MudarNumero(){
  tone(pinoBuzzer,10000,15);
  delay(100);
  tone(pinoBuzzer,5000,15);
  delay(100);  
}

// Apertar botão de tentativa
void PressButton(){
  tone(pinoBuzzer,7000,15);
  delay(300);
  tone(pinoBuzzer,9000,15);
  delay(300);  
}

// Tocar som fim de jogo
void GameOverSong(){
  tone(pinoBuzzer,7000,15);
  delay(300);
  tone(pinoBuzzer,9000,15);
  delay(300);  
}

void PrintTelaJogo(){
    lcd_1.clear();
    lcd_1.setCursor(0, 0);
    lcd_1.print("CHUTE:");
    lcd_1.print(chute);
    lcd_1.setCursor(6, 1);
    lcd_1.print("CHANCES:");
    lcd_1.print(chances);
}

void salvarPartida(){

  uint8_t posicaoLetra=0;
  char nomeJogador[]="AAA";
  char letra=65;

  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("NOME:");
  
  while(posicaoLetra<3){

    lcd_1.setCursor(5, 0);
    lcd_1.print(nomeJogador);

    letra+=+(analogRead(Vx) <= 300 && letra < 90 )-(analogRead(Vx) >= 700 && letra > 65);
    
    nomeJogador[posicaoLetra]=letra;
    if(digitalRead(botaoJoystick)==LOW){
      posicaoLetra++;
    }

    delay(200);
  }

  pacoteDados["placar"]=placarJogo;
  pacoteDados["nome"]=String(nomeJogador);
  serializeJson(pacoteDados, Serial);
  Serial.println();
}

void setup()
{
  Serial.begin(9600);
  
  lcd_1.init();
  lcd_1.clear();
  lcd_1.backlight();

  pinMode(botaoJoystick,INPUT_PULLUP);
  
  pinMode(pinoBuzzer,OUTPUT);
  
  dificuldadeJogo=0;
  modoJogo=0;
  chute=0;
  correto=0;
  chances=0;
}


void loop()
{
  if(modoJogo==0){
    TelaInicial();
  }else{
    PrintTelaJogo();
    TelaJogo();
  }
}

void TelaInicial(){
  if(dificuldadeJogo>=3 && chances < 1){

  }
  
  lcd_1.clear();
  lcd_1.setCursor(0, 0);
  lcd_1.print("ESCOLHA NIVEL:");
  while(true){
    delay(500);
    lcd_1.setCursor(5, 1);

    //Printar a dificuldade
    switch(dificuldadeJogo){
      case 0:
          lcd_1.print("FACIL  ");
      break;
      case 1:
          lcd_1.print("MEDIO  ");
      break;
      case 2:
          lcd_1.print("DIFICIL");
      break;
    }


    if(analogRead(Vx) <= 300 && dificuldadeJogo < 3){
       MudarNumero();
      dificuldadeJogo++;   //Aumentar dificuldade
    }else if(analogRead(Vx) >= 700 && dificuldadeJogo > 0){
      MudarNumero();
      dificuldadeJogo--;  //Diminuir dificuldade
    }

    if(digitalRead(botaoJoystick) == LOW){
      PressButton();
      
      if(dificuldadeJogo < 2)dificuldadeJogo++;
      
      correto = random(((dificuldadeJogo)*10)+1);
      chances = 10/(dificuldadeJogo+1);
      modoJogo=1;//Ir pro jogo
      break;
    }
    delay(250);
  }
}

void TelaJogo(){
  while(true){
    delay(500);
    uint8_t oldChute = chute;
     
    
    if(analogRead(Vx) <= 300 && chute+1 < (dificuldadeJogo*10)+1){
      chute++;
      MudarNumero();
    }else if(analogRead(Vx) >= 700){
      chute--;
      MudarNumero();
    }

    if(oldChute!=chute){
      lcd_1.setCursor(6, 0);
      lcd_1.print("   ");
      lcd_1.setCursor(6, 0);
      lcd_1.print(chute);
    }
    
    if(digitalRead(botaoJoystick)==LOW){
      PressButton();
      
      if(chute==correto){
        lcd_1.clear();
        lcd_1.setCursor(4, 0);
        lcd_1.print("PARABENS");

        delay(3000);
        modoJogo=0;
        placarJogo++;
        break;  //Voltar ao menu
      }        
      else if(chances--<1){
        lcd_1.clear();
        lcd_1.setCursor(2, 0);
        lcd_1.print("GAME OVER");
        lcd_1.setCursor(3, 1);
        lcd_1.print("RESULTADO:"); 
        lcd_1.print(correto);
        GameOverSong(); //Chamar música do game over
        
        
        delay(2000);

        salvarPartida();
        placarJogo=0;
        dificuldadeJogo=0;
        modoJogo=0;
        break;  //Voltar ao menu
      }else if(chute > correto){

        lcd_1.clear();
        lcd_1.setCursor(5, 0);
        lcd_1.print("MENOR");   //Printar maior na tela    
      }else{
        lcd_1.clear();
        lcd_1.setCursor(5, 0);
        lcd_1.print("MAIOR");    //Printar menor na tela     
      }
      delay(2000); 
      PrintTelaJogo();
    }
    
  }
}
