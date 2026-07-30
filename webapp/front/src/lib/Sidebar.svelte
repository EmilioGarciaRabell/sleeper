<aside class="sidebar">
  <div class="sidebar-top">
    <span class="app-label">APP</span>
    <h2>Voice Notes Diary</h2>
    
    <!-- Add your search input here -->
    <div class="search-placeholder">Search...</div>

    <nav>
      <ul>
        <li class="active">Timeline</li>
        <li>Search</li>
        <li>Calendar</li>
        <li>Settings</li>
      </ul>
    </nav>
  </div>

  <div class="sidebar-bottom">
    <small>DEVICE: ONLINE</small>
    <p>Nano ESP32</p>
  </div>
</aside>

<style>
  .sidebar {
    width: 260px;
    height: 100vh;
    background-color: #141416;
    color: #fff;
    display: flex;
    flex-direction: column;
    justify-content: space-between;
    padding: 24px 16px;
    position: fixed;
    left: 0;
    top: 0;
  }
  .app-label {
    font-size: 10px;
    color: #6b7280;
  }
  .search-placeholder {
    background: #1f2024;
    padding: 8px;
    border-radius: 6px;
    color: #6b7280;
    margin: 16px 0;
    font-size: 13px;
  }
  nav ul {
    list-style: none;
    padding: 0;
    display: flex;
    flex-direction: column;
    gap: 8px;
  }
  nav li {
    padding: 8px 12px;
    border-radius: 6px;
    color: #9ca3af;
    cursor: pointer;
  }
  nav li.active, nav li:hover {
    background: #1f2024;
    color: #fff;
  }
  .sidebar-bottom {
    background: #1b1c20;
    padding: 12px;
    border-radius: 8px;
  }
</style>