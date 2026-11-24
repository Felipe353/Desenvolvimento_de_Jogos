

    BehaviorTree Arvore (

      Selector(

          Sequence(Condition("Passou"), Action("FechePorta")),

          Sequence(Condition("NaPorta"), Condition("PortaAberta"), Action("Passe")),

          Sequence(Condition("NaPorta"), Condition("PortaDestrancada"), Condition("PortaFechada"), Action("AbrirPorta")),

          Sequence(Condition("NaPorta"), Condition("PortaTrancada"), Action("DestranquePorta")),

          Action("CaminhePorta")
        )
    );


    context = {
        {"NaPorta", 1},
        {"PortaTrancada", 1},
        {"PortaFechada", 1},
    };

    std::cout << Arvore.tick(context) << '\n';

