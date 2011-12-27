<?php
/**
 * Dec 21, 2011
 * @file og_simplenews.features.menu_links.inc
 * @author blazey http://drupal.org/user/353861
 */

/**
 * Implementation of hook_menu_default_menu_links().
 */
function og_simplenews_feature_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: features:newsletter
  $menu_links['features:newsletter'] = array(
    'menu_name' => 'features',
    'link_path' => 'newsletter',
    'router_path' => 'newsletter',
    'link_title' => 'Newsletter',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );

  return $menu_links;
}