<template>
  <div id="map" style="height: 100vh;"></div>
</template>

<script>
import L from 'leaflet';
import 'leaflet/dist/leaflet.css';

export default {
  name: 'OsmMap',
  data() {
    return {
      map: null, // 将map添加到data中，以便在组件的其他部分使用
    };
  },
  mounted() {
    this.initMap();
  },
  methods: {
    initMap() {
      const options = {
        center: [23.061977, 113.392585], // 地图中心
        minZoom: 0,
        maxZoom: 18,
        zoom: 10,
        zoomControl: true, // 禁用 + - 按钮
        doubleClickZoom: true, // 禁用双击放大
        dragging: true, // 禁止鼠标拖动滚动
        boxZoom: true, // 决定地图是否可被缩放到鼠标拖拽出的矩形的视图，鼠标拖拽时需要同时按住shift键
        scrollWheelZoom: true, // 禁止鼠标滚动缩放
        zoomSnap: 0.5, // 地图能放缩的zoom的最小刻度尺度，默认值1
        fullscreenControl: true, // 全屏控件，不显示
      };
      this.map = L.map('map', options); // 初始化地图并赋值给data中的map

      // 添加创建地图的函数
      const createTileLayer = (map, url, options) => {
        let tileLayer = L.tileLayer(url, options);
        tileLayer.addTo(map);
        return tileLayer;
      };

      // 创建并添加瓦片图层
      createTileLayer(this.map, "https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png", {
        attribution: 'Map data © <a href="https://www.openstreetmap.org">OpenStreetMap</a> contributors',
      });
    },
  },
};
</script>