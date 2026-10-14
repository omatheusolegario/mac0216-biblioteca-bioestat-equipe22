/**
 * @brief Capacidade do teste de identificar corretamente quem tem a doença
 * @param vp Verdadeiros positivos
 * @param fn Falsos negativos
 * @pre vp+fn != 0
 * @return Proporção de testes positivos entre quem tem a doença
 */
double sensibilidade(int vp, int fn);

/**
 * @brief Capacidade do teste de identificar corretamente quem não tem a doença
 * @param vn Verdadeiros negativos
 * @param fn Falsos positivos
 * @pre vn+fp != 0
 * @return Proporção de testes negativos entre quem não tem a doença
 */
double especificidade(int vn, int fp);

/**
 * @brief Probabilidade da pessoa realmente ter a doença quando o resultado é positivo
 * @param vp Verdadeiros positivos
 * @param fp Falsos positivos
 * @pre vp+fp != 0
 * @return Proporção de pessoas com a doença entre os resultados positivos
 */
double vpp(int vp, int fp);

/**
 * @brief Probabilidade da pessoa realmente não ter a doença quando o resultado é negativo
 * @param vn Verdadeiros negativos
 * @param fn Falsos negativos
 * @pre vn+fn != 0
 * @return Proporção de pessoas sem a doença entre os resultados negativos
 */
double vpn(int vn, int fn);