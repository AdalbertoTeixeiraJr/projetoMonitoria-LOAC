package monitor
/*
 *Temos que começar a fazer os java docs de cada classe, senão vai ficar muito dificil, fazer tudo depois,
 *vamos documentar.
*/
class Monitor {
    String nome//?, matricula, curso, email, celular, telFixo, disciplina, unidade, codigoDoProjeto, professorOrientador
    String sobrenome
    String matricula
    String curso
    String cpf
    String email
    String Identidade
    String celular
    String telFixo
    String senha
    String nomeMae
    String nomePai
    String disciplina
    String unidade
    String codigoDoProjeto
    String professorOrientador
    String periodosMonitorVoluntario
    String periodosMonitorBolsista
    String avalicaoComplementar
    String sexo
    String hashSenha
    float notaDisciplina, cra, notaObtida, mediaFinal
    int classificacao

    // falta colocar métodos
    //ainda não sei como funciona os métodos nessa linguagem
    //depois tenho que parar pra pesquisar.
    static constraints = {
        nome(blank:false)
        sobrenome(blank:false)
        matricula(blank:false)
        curso(blank:false)
        cpf(blank:false)
        email(email:true)
        identidade(blank:false)
        senha(blank:false)
        

    }
    void setSenha(String valor){
        this.senha = valor;
        if (valor != null) this.hashSenha = valor.encodeAsPassword()

    }
    static transients = ['senha']
}