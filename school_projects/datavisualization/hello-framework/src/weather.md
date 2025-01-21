# Weather report



```js
import {smallNumber} from "./components/smallNumber.js";
```

```js
const forecast = FileAttachment("./data/forecast.json").json();
```

```js
const gistemp = FileAttachment("./data/gistemp.csv").csv({typed: true})
```


<div class="grid grid-cols-1">
  <div class="card">${resize((width) => temperaturePlot(forecast, {width}))}</div>
</div>

<div class="grid grid-cols-1">
  <div class="card">${resize((width) => divergingscatter(gistemp, {width}))}</div>
</div>

<div class="grid grid-cols-2" style="grid-auto-rows: 504px;">
  <div class="card">${
    resize((width) => Plot.plot({
      title: "How big are penguins, anyway? 🐧",
      width,
      grid: true,
      x: {label: "Body mass (g)"},
      y: {label: "Flipper length (mm)"},
      color: {legend: true},
      marks: [
        Plot.linearRegressionY(penguins, {x: "body_mass_g", y: "flipper_length_mm", stroke: "species"}),
        Plot.dot(penguins, {x: "body_mass_g", y: "flipper_length_mm", stroke: "species", tip: true})
      ]
    }))
  }</div>

  <div class="card">${
    resize((width) => Plot.plot({
      title: "Your awesomeness over time 🚀",
      subtitle: "Up and to the right!",
      width,
      y: {grid: true, label: "Awesomeness"},
      marks: [
        Plot.ruleY([0]),
        Plot.lineY(aapl, {x: "Date", y: "Close", tip: true})
      ]
    }))
  }</div>

</div>

<div class="grid grid-cols-2">
  <div class="card grid-colspan-2">one–two</div>
  <div class="card">three</div>
  <div class="card">four</div>
  <div class="card">five</div>
</div>



${selectIslandSegmentInput}

## All Selections: ${selectIslandSegment}

<div class="grid grid-cols-4">
    <div class="card grid-rowspan-1">
        ${resize((width) => smallNumber(`Selections: ${selectIslandSegment}`, width))}
    </div>
</div>

```js
function temperaturePlot(data, {width} = {}) {
  return Plot.plot({
    title: "Hourly temperature forecast",
    width,
    x: {type: "utc", ticks: "day", label: null},
    y: {grid: true, inset: 10, label: "Degrees (F)"},
    marks: [
      Plot.lineY(data.properties.periods, {
        x: "startTime",
        y: "temperature",
        z: null, // varying color, not series
        stroke: "temperature",
        curve: "step-after"
      })
    ]
  });
}
```

```js
function divergingscatter(data, {width} = {}) {
  return Plot.plot({
    y: {
      grid: true,
      tickFormat: "+f",
      label: "↑ Surface temperature anomaly (°C)"
    },
    color: {
      scheme: "BuRd",
      legend: true
    },
    marks: [
      Plot.ruleY([0]),
      Plot.dot(gistemp, {x: "Date", y: "Anomaly", stroke: "Anomaly"})
    ]
  })
}
```



```js
// Radio button input to choose market segment
const pickIslandSegmentInput = Inputs.radio(
  ["All"].concat(penguins.filter((d) => d.island != "Torgersen").map((d) => d.island)),
  {
    label: "Island:",
    value: "All",
    unique: true
  }
);
const pickIslandSegment = Generators.input(pickIslandSegmentInput);

const selectIslandSegmentInput = Inputs.checkbox(
  ["All"].concat(penguins.filter((d) => d.island != "Gentoo").map((d) => d.island)),
  {
    label: "Island:",
    value: "All",
    unique: true
  }
);
const selectIslandSegment = Generators.input(selectIslandSegmentInput);
```


```js
display(penguins);
```