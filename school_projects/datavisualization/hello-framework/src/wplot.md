# w report




```js
import {smallNumber} from "./components/smallNumber.js";
import {countryInterchangeChart, top5BalancingAuthoritiesChart, usGenDemandForecastChart} from "./components/charts.js";
```

```js
const forecast = FileAttachment("./data/forecast.json").json();
const usDemandGenForecastAll = FileAttachment("./data/us-demand.csv").csv({typed: true});
const baHourlyDemand = FileAttachment("./data/eia-ba-hourly.csv").csv({typed: true});

const ministryFile = FileAttachment("./data/ministry-contract-min.csv").csv({typed: true});
```


```js
// Omit total interchange
const usDemandGenForecast = usDemandGenForecastAll.filter(d => d.name != "totalInterchange");
```


```js
// Date/time format/parse
const timeParse = d3.utcParse("%Y-%m-%dT%H");
const hourFormat = d3.timeFormat("%-I %p");

const MS_IN_AN_HOUR = 1000 * 60 * 60;
const hours = [...new Set(baHourlyDemand.map(d => d.period))].map(timeParse);
const [startHour, endHour] = d3.extent(hours);

//NEW STUFF: GROUPING

const dawndata = [
  {Province:"BC", industry: "Commun", year: "2019", date: "2019-10-10", value: 32.3104},
  {Province:"BC", industry: "Commun", year: "2019", date: "2019-12-29", value: 190.4876},
  {Province:"ON", industry: "Infra", year: "2019", date: "2019-01-02", value: 400.1440},
  {Province:"QC", industry: "Infra", year: "2020", date: "2020-12-21", value: 100.0},
  {Province:"ON", industry: "Bussi", year: "2020", date: "2020-12-21", value: 347.1200},
  {Province:"FL", industry: "GovLaw", year: "2020", date: "2019-01-02", value: 304.6},
  {Province:"NY", industry: "GovLaw", year: "2018", date: "2019-01-02", value: 600.6}
]


const sortedDataT = dawndata.sort((a, b) => d3.ascending(a.year, b.year));
const sortedDataP = ministryFile.sort((a, b) => d3.ascending(a.Province, b.Province));


const gByYearDateT = d3.rollups(
  sortedDataT, 
  (v) => d3.sum(v, (d) => d.value), 
  (d) => d.year)
  .map(([name, value]) => ({name, value}));



// USE THIS
const mpByProvT = d3.group(sortedDataP, d => d.Province, (d) => d.ShortIndustry);
// const specProv = mpByProvT.get("Connecticut");
const specProv = mpByProvT.get("British Columbia");
const specProvInd = []

const iterator1 = specProv.values();

for (let i = 0; i < specProv.size; i++){
  // an array  of 
  // const mapValue = specProvInd.push(iterator1.next().value);
  iterator1.next().value.forEach((element) => specProvInd.push(element))
}

// array1.forEach((element) => console.log(element));

const selectedProvIndGrps = d3.rollups(
  specProvInd, 
  (v) => d3.count(v, (d) => d.Amendments), 
  (d) => d.ShortIndustry)
  .map(([name, value]) => ({name, value}));


selectedProvIndGrps.sort((a, b) => b.value - a.value);
// const selectName = selectedProvIndGrps[0].name

selectedProvIndGrps[0].name = "COS"
// FUNCTION
// END OF STUFF TO USE

const gByPrvNindr = d3.rollups(
  sortedDataP, 
  (v) => d3.count(v, (d) => d.Amendments),
  (d) => d.Province,
  (d) => d.ShortIndustry);

// const bookingsAll = gByPrvNindr.filter((d) => d.Province == "Colorado");

const gMinstrByProvT = d3.group(ministryFile, d => d.Province);




const rawdata = [
  {ot: "i", area: "1", date: new Date("2019-10-10"), value: 32.3104},
  {ot: "i", area: "1", date: new Date("2019-12-29"), value: 190.4876},
  {ot: "i", area: "2", date: new Date("2019-01-02"), value: 400.1440},
  {ot: "j", area: "2", date: new Date("2020-12-21"), value: 100.0},
  {ot: "j", area: "1", date: new Date("2020-12-21"), value: 347.1200},
  {ot: "j", area: "1", date: new Date("2020-12-18"), value: 500.00},
  {ot: "j", area: "2", date: new Date("2019-01-02"), value: 30.6,}
]


const sortedData = rawdata.sort((a, b) => d3.descending(a.area, b.area));
const gByYearDate = d3.rollup(
  rawdata, 
  (v) => d3.count(v, (d) => d.area),
  (d) => d.ot,
  (d) => d.date.getFullYear()
);

//END OF NEW STUFF


const hoursBackOfData = Math.ceil(Math.abs(endHour - startHour) / (MS_IN_AN_HOUR)) - 1;

const hoursAgoInput = Inputs.range([hoursBackOfData, 0], {step: 1, value: 0, width: 150});
const hoursAgo = Generators.input(hoursAgoInput);
hoursAgoInput.querySelector("input[type=number]").remove();
```

```js
// Get current date in readable format
const formatDate = d3.utcFormat("%B %d, %Y");
const currentHour = new Date(endHour.getTime() - hoursAgo * MS_IN_AN_HOUR);
const currentDate = d3.timeFormat("%-d %b %Y")(currentHour);
```

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
        Plot.linearRegressionY(penguins, {x: "body_mass_g", y: "flipper_length_mm", stroke: "island"}),
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

<div class="grid grid-cols-1">
  <div class="card grid-colspan-1">one–two</div>
  <div class="card">three</div>
  <div class="card">four</div>
</div>


${pickIslandSegmentInput}

## Picked Island: ${pickIslandSegment}

<div class="grid grid-cols-4">
    <div class="card grid-rowspan-1">
        ${resize((width) => smallNumber(`Selections: ${pickIslandSegment}`, width))}
    </div>
</div>


${selectIslandSegmentInput}

## All Selections: ${selectIslandSegment}

<div class="grid grid-cols-4">
    <div class="card grid-rowspan-1">
        ${resize((width) => smallNumber(`First Selection: ${selectIslandSegment[0]}`, width))}
    </div>
</div>

<div class="grid grid-cols-4">
    <div class="card grid-rowspan-1">
        ${resize((width) => smallNumber(`Second Selection: ${selectIslandSegment[1]}`, width))}
    </div>
</div>

<div class="grid grid-cols-4">
    <div class="card grid-rowspan-1">
        ${resize((width) => smallNumber(`Second Selection: ${selectIslandSegment[2]}`, width))}
    </div>
</div>



<!-- <div class="grid grid-cols-4">
  <div class="card grid-colspan-2 grid-rowspan-3">
    <div style="display: flex; flex-direction: column; align-items: center;">
        <h1 style="margin-top: 0.5rem;">${hourFormat(currentHour)}</h1>
        <div>${currentDate} </div>
        <div style="display: flex; align-items: center;">
          <div>-${hoursBackOfData} hrs</div>
          ${hoursAgoInput}
          <div style="padding-left: 0.5rem;">now</div>
        </div>
      </div>
  </div>

  <div class="card grid-colspan-2">
    <h2>Top 5 balancing authorities by demand on ${currentDate} at ${hourFormat(currentHour)} (GWh)</h2>
  </div>
  
  <div class="card grid-colspan-2">
    <h2>Neighboring country interchange (GWh)</h2>
  </div>

  <div class="card grid-colspan-2">
    <h2>US electricity generation demand vs. day-ahead forecast (GWh)</h2>
    ${resize((width, height) => usGenDemandForecastChart(width, height, usDemandGenForecast, currentHour))}
  </div>

  <div class="card grid-colspan-2">
    <h2>Neighboring country interchange (GWh)</h2>
  </div>
</div> -->


<div class="grid grid-cols-1" style="grid-auto-rows: 100px;">
  <div class="card">
      <h3 style="margin-top: 0.5rem;">Selected year: ${selectedYr}</h3>
      <div style="display: flex; align-items: center;">
        <div>Select a year</div>
        ${yrsInput}
        <div style="padding-left: 0.5rem;"> data</div>
      </div>
  </div>
</div>

## Hours Ago Input
```js
const yrsInput = Inputs.range([2012, 2018], {step: 1, value: 0, width: 150});
const selectedYr = Generators.input(yrsInput);
yrsInput.querySelector("input[type=number]").remove();

// display(hrsAgoInput)
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
const defaultIslandZero = ["Torgersen", "Biscoe"];
const defaultIslandOne = [];


const selectIslandSegmentInput = Inputs.checkbox(
  ["All"].concat(penguins.filter((d) => d.island != "Gentoo").map((d) => d.island)),
  {
    label: "Island:",
    value: defaultIslandZero,
    unique: true
  }
);
const selectIslandSegment = Generators.input(selectIslandSegmentInput);
```




```js
display(selectIslandSegment)
```

```js
// display(endHour.getYear())
// display(endHour.setYear(124))
display(baHourlyDemand)
display(sortedDataT)
display(gMinstrByProvT)
// display(newHoursTwo[3])
// display(newHoursTwo[3].getMonth())
// display(hours)
display(gByYearDateT)
```
## Award Total
```js
display(gByPrvNindr)
// display(gByPrvNindr[5][0])
display(mpByProvT)
// display(gByYearDate)
```

```js

const colorsCheck = Inputs.checkbox(["red", "green", "blue"], {label: "color"})
display(specProv)
display(specProvInd)
// display(rawdata)
display(selectedProvIndGrps)
// display(selectName)
```






```js
/*const selectIslandSegmentInput = Inputs.checkbox(
  ["All"].concat(co.filter((d) => d.island != "Gentoo").map((d) => d.island)),
  {
    label: "Island:",
    value: "All",
    unique: true
  }
);
const selectIslandSegment = Generators.input(selectIslandSegmentInput);*/
```



