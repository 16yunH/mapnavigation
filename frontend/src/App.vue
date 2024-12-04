<template>
  <div id="app">
    <OsmMap />
  </div>
</template>

<script>
import OsmMap from './components/mapview.vue';

export default {
  name: 'App',
  components: {
    OsmMap,
  },
};
</script>

<style  scoped>
.main_map {
  height: calc(100vh - 50px) !important;
  width: 100%;
}
</style>