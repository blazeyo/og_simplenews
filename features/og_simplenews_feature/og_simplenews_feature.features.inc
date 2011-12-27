<?php
/**
 * Dec 21, 2011
 * @file og_simplenews_feature.features.inc
 * @author blazey http://drupal.org/user/353861
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function og_simplenews_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "boxes" && $api == "box") {
    return array("version" => 1);
  }
  elseif ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}