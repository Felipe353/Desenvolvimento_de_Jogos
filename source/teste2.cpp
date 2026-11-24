

    BehaviorTree Arvore (

      Selector(

          Sequence(Condition("ChegouPontoA"), Action("InverterDirecao")),

          Sequence(Condition("ChegouPontoB"), Action("InverterDirecao")),

          Sequence(Condition("IndoParaA"), Action("AndeParaA")),

          Sequence(Condition("IndoParaB"), Action("AndeParaB"))

        )
    );


    Estado_do_Jogo = {
        {"IndoParaA", 1},
    };



//-------------------------------------

void Execucao_Meu_NPC () {


     flag direcaoA = true;


     if (direcaoA)
        Estado_do_Jogo = {{"IndoParaA", 1}};
     else
        Estado_do_Jogo = {{"IndoParaB", 1}};



     comando = Arvore.tick (Estado_do_Jogo)
 

     if ("InverterDirecao" == comando)  direcaoA = !direcaoA;
     if ("AndeParaA" == comando)        x = x + 1;
     if ("AndeParaB" == comando)        x = x - 1;

}

//------------------------------------------------

void GameLoop () {

   while (true) {

     Execucao_Meu_NPC ();

     Mostrar_Todos_NPCs_na_Tela ();

   }

}     






     

  }
