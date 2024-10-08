---
title: Add styles to HTML element
tags: browser
expertise: beginner
author: chalarangelo
firstSeen: 2021-01-07T00:37:43+02:00
lastUpdated: 2021-01-07T00:37:43+02:00
---

Adds the provided styles to the given HTML element.

- Use `Object.assign()` and `ElementCSSInlineStyle.style` to merge the provided `styles` object into the style of the given element.

```js
const addStyles = (el, styles) => Object.assign(el.style, styles);
```

```js
addStyles(document.getElementById('my-element'), {
  background: 'red',
  color: '#ffff00',
  fontSize: '3rem'
});
```
