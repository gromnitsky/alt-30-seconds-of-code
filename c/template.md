---
title: Safe strlen
---

Explain briefly what the snippet does.

- Explain briefly how the snippet works.
- Use bullet points for your snippet's explanation.
- Try to explain everything briefly but clearly.

```c
int safe_strlen(char *s) {
   return s ? strlen(s) : 0;
}
```

```c
safe_strlen(NULL); // 0
```
