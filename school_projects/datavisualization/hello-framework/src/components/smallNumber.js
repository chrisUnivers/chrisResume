import {html} from "npm:htl";

export function smallNumber(metric, width) {
  return html`
  <h2>${metric}</h2>
  <div style="font-size: ${width / 250}rem">
  <div>`;
}
