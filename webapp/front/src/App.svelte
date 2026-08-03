<script>
  import Sidebar from './lib/Sidebar.svelte';
  import TimelineView from './lib/TimelineView.svelte';
</script>

<main class="app-layout">
  <Sidebar />
  <section class="main-content">
    <TimelineView />
  </section>
</main>

<style>
  .app-layout {
    display: flex;
    min-height: 100vh;
  }

  .main-content {
    margin-left: 260px; /* matches sidebar width */
    flex-grow: 1;
    background-color: #f7f6f3;
  }
</style>