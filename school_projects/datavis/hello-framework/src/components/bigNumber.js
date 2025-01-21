import {html} from "npm:htl";

export function bigNumber(metric, dateArray, value, compare, width) {
  return html`
  <h2>${metric}</h2>
  <div style="font-size: ${width / 250}rem">
  <h3><i>${dateArray[0]} to ${dateArray[1]}</i></h3>
  <h1>${value}</h1>
  <div>${compare}</div>
  <div>`;
}

export function greatNumber(metric, descript, width) {
  return html`
  <h2 style="font-weight: bold">${metric[0]}:</h2>
  <div style="font-size: ${width / 250}rem">
  <h3><i>${descript}</i></h3>
  <div>`;
}

export function geniusNumber(metric, combineRev, width) { 
  // const dollarFormat = d3.format("$.2f");
  // ${dollarFormat(d.value)}
  // <h1 style="font-weight: bold; color: black" >${dollarFormat(d.value)}</h1>
  return html`
  <h3 style="font-weight: bold">Combined rev in Millions</h3>
  <h2 style="font-weight: bold">${metric[0]}</h2>
  <div style="height: ${width / 250}px">
  <h1 style="font-weight: bold; color: black" >${combineRev}</h1>
  <div>`;
}
