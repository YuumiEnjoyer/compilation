|           | <LETTER> | <DIGIT> |  .  | <оп> | < > |
|-----------|-----|-----|-----|------|-----|
| START     |  ID  |  INT  |     |  OP   |  START  |
| ID        |  ID  |  ID  |     |  END   |  END  |
| INT       |     |  INT  |  FLOAT_DOT  |  END   |  END  |
| FLOAT_DOT |     |  FLOAT  |     |      |     |
| FLOAT     |     |  FLOAT  |     |  END   |  END  |
| OP        |  END  |  END  |     |  END   |  END  |
